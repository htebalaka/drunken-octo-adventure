#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sstream>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include"../headers/sockets.h"
#include "../headers/globalConstants.h"
#include "../headers/Zenity.h"
using namespace std;

// zenity doesn't always work well over ssh, so setting this to
// false will fall back to using cin/cout for setting up the
// networking
bool useZenity = false;

/**********************************************************************************************
      Gets Ip address data for socket
      self contained, not to be used out of server.cpp
**********************************************************************************************/
void *get_in_addr(struct sockaddr *sa)//get socket address IPv4 or IPv6
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
/**********************************************************************************************
   Socket handler
**********************************************************************************************/
void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}
/**********************************************************************************************
   client_Connect, connects the client to an availible game.
   Input: none, Fetches data from user itself, could be changed to receive userdata in
   output: game_info struct with player data
**********************************************************************************************/
game_Info client_Connect(){
	game_Info gameData;

   string userName = Zenity::getUserText("", "Please enter a user name:", "", useZenity);

   /*
	cout << "Please Enter a Username:\n";
   string userName;
   cin >> userName;
   */
	gameData = get_Game();
	string sport ;
	stringstream out;
	out << gameData.port;
	sport = out.str();
	string opponent = gameData.userName; //the opponents name is what is currently in gameData
	int sockfd;  
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((rv = getaddrinfo((gameData.host).c_str(), sport.c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	}
	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			exit(EXIT_FAILURE);
		}
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			exit(EXIT_FAILURE);
		}
		break;
	}
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
	freeaddrinfo(servinfo); // all done with this structure
	char uname[MAXDATASIZE];
	fillarray(userName, uname);
	send(sockfd, uname, MAXDATASIZE, 0);
	char recm[MAXDATASIZE];
	recv(sockfd, recm, MAXDATASIZE, 0 );
	if(recm[0] == 'Y'){
		if(clear_Game(gameData.name)){//clear the game from the pending game list and start.
			gameData.userName = userName;
			gameData.opponent = opponent;
			gameData.sockfd = sockfd;
		}else{
			cout << "ERROR Clearing Game!\n";//if we c
		}
	}else{
		cout << "Host declined your game request, Please Try again\n";
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	return gameData;
}
/**********************************************************************************************
  sync_Board, Passes starting board data to each player respectively
   input: players board data, game_Info struct
   output: opponents board data
**********************************************************************************************/
void sync_Board(string board, game_Info gameData, string &positions){
	char opponentBoard[MAXDATASIZE];
	char position[MAXDATASIZE];
   fillarray(board, position);
   switch(gameData.playerType){//sockets are a 1 way street, cant send and recieve at the same time...
      case 'B'://host  always sends first
         send(gameData.sockfd, position , MAXDATASIZE, 0);//send board data to client
         recv(gameData.sockfd, opponentBoard , MAXDATASIZE, 0);//recieve clients board data
      break;
      case 'R'://client recieves first
         recv(gameData.sockfd, opponentBoard , MAXDATASIZE, 0);//recieve hosts board data
         send(gameData.sockfd, position , MAXDATASIZE, 0);//send board data to host           
      break;
      default://something went wrong!
         cout << "ERROR, Unable to determine player type!\n";
   		return;
         break;
   }
	positions = opponentBoard;
	return;
}
/**********************************************************************************************
  send_move, Passes move data to other player
   input: move data, game_Info struct, turn bool
   output: true if sent, false it it wasnt your turn
**********************************************************************************************/
bool send_Move(string move, game_Info gameData, bool &turn){
	char movedata[MAXDATASIZE];
		
	fillarray(move, movedata);
	
	if(turn){//it is your turn!	
		send(gameData.sockfd, movedata , MAXDATASIZE, 0);
		turn = false;
		return true;
	}else{
		return false;
	}
}

string get_Move(game_Info gameData, bool &turn){
		char data[MAXDATASIZE];
		recv(gameData.sockfd, data , MAXDATASIZE, 0);
		string oMove = data;
		turn = true;
		return oMove;
}
/**********************************************************************************************
   host_Connect, creates a new game and waits for a player 2
   input: none, handles user input itself
   output: game_Info struct with player data
**********************************************************************************************/
game_Info host_Connect(){
	srand (time(NULL));
	int choose = rand() % 100 + 4000;
	int port = choose;//picks random port

   string userName = Zenity::getUserText("", "Please enter a user name:", "", useZenity);
   string name = Zenity::getUserText("", "Please enter a name for your game:", "", useZenity);

   /*
   cout << "Please Enter a Username:\n";
   string userName;
   cin >> userName;
	cout << "Please Enter a Name for your Game:\n";
	string name;
	cin >> name;
   */
   char hostname[128];
   gethostname(hostname, 127);
   game_Info gameData = create_Game(port,hostname,name, userName); 
	int new_fd = -1;
	new_fd = wait_Game(gameData, false);//first pass we must bind the socket
	while(new_fd == -1){    
		new_fd = wait_Game(gameData, true);//if we must do another pass skip binding
	}
	gameData.sockfd = new_fd;
	return gameData;
	
}
/**********************************************************************************************
   wait_Game: Listen for client, connect if accepted, if client is denied return -1;
	input: reference to game_Info struct, boolean to determine if binding is needed
**********************************************************************************************/
int wait_Game(game_Info &gameData, bool reload){
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	char s[INET6_ADDRSTRLEN];
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	string sport ;
	stringstream out;
	out << gameData.port;
	sport = out.str();
	int yes=1;
	int rv;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	if ((rv = getaddrinfo(NULL, sport.c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	}
		// loop through all the results and bind to the first we can
	if(!reload){
		for(p = servinfo; p != NULL; p = p->ai_next) {
			if ((sockfd = socket(p->ai_family, p->ai_socktype,
					p->ai_protocol)) == -1) {
				perror("server: socket");
				continue;
			}
			if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
				perror("setsockopt");
				exit(1);
			}
	
			if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
				close(sockfd);
				perror("server: bind");
				continue;
			}
			break;
		}
		if (p == NULL)  {
			fprintf(stderr, "server: failed to bind\n");
		}
	}else{
		sockfd = gameData.sockfd;
	}
	freeaddrinfo(servinfo); // all done with this structure
	char action = 'q';
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	printf("server: waiting for connections...\n");
	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}
		inet_ntop(their_addr.ss_family,
		get_in_addr((struct sockaddr *)&their_addr),
		s, sizeof s);
		if (!fork()) { // this is the child process
			char opponent[MAXDATASIZE];
			recv(new_fd, opponent, MAXDATASIZE,0);
         string opponentString = opponent;
         action = Zenity::getAnswer("", "Accept game request from" + opponentString + "?", "Yes", "No", useZenity) ? 'Y' : 'N';
         /*
			cout << "Accept Game request from: " << opponent << "? (Y,N)\n";
			cin >> action;
         */
			bool trip = false;
			do{
				switch (action){
					case 'Y':
						gameData.opponent = opponent;
						close(sockfd);
						send(new_fd,"Y", MAXDATASIZE, 0);
						trip = true;
						return new_fd;
					break;
					case 'N':
						send(new_fd, "N", MAXDATASIZE, 0);
						gameData.sockfd = sockfd;
						close(new_fd);
						sleep(1);
						return -1;
						trip = true;
					break;
					default:
						cout << "Invalid Response, Try Again!\n";
					break;
				}
			}while(trip == false);
		}
	}
}	
/**********************************************************************************************
   get_Game
   Fetch all games waiting for a player 2, Prompt user to select one
   input: none
   output: game_info struct
**********************************************************************************************/
game_Info get_Game(){
	string line;
	ifstream gamefile("/home/student/marwoomd/Public/connections.txt");
	int numGames = 0;
	if(gamefile.is_open()){//get number of availible games
		while(!gamefile.eof()){
			getline(gamefile,line);
			if(line[0] == '4'){
				numGames++;
			}
		}
		gamefile.close();
	}
	game_Info * games = new game_Info[numGames];
	int select = 0;
	ifstream gameData("/home/student/marwoomd/Public/connections.txt");
	if(gameData.is_open()){//go through file and add each game to an array of game_Info structs
		while(!gameData.eof()){
			getline(gameData,line);
			if(line[0] == '4'){
				istringstream iss(line);
				string word;
				iss >> games[select].port;
				iss >> games[select].host;
				iss >> games[select].name;
         	iss >> games[select].userName;
				select++;
			}
		}
	}
	gamefile.close();
	int dGame;
	if(numGames > 0){
		cout << "Availible Games:\n";
		cout << "Games:" << numGames << endl;
		for(int i=0;i < (numGames);i++){//display list of games to user
			cout << i << " : " << games[i].name << endl;
		}
		cout << "Enter Game: ";//require user to enter desired game number
		
		do{
			cin >> dGame;
			if((dGame < 0) || (dGame > (numGames-1))){//make sure selection is valid
				cout << "**ERROR Invalid Game Selection!\nPlease Enter Game: ";
			}
		}while((dGame < 0) || (dGame > (numGames-1)));
	}else{
		cout << "**ERROR No Games Availible!\n";
		exit(EXIT_FAILURE);
	}
	return games[dGame];
}
/**********************************************************************************************
   clear Game, removes game from list of availible games
   input: name of game to clear as a string
   output: boolean true if successful
**********************************************************************************************/
bool clear_Game(string name){
	string line;
	ifstream gamefile("/home/student/marwoomd/Public/connections.txt");
	int numGames = 0;
	if(gamefile.is_open()){// get number of games
		while(!gamefile.eof()){
			getline(gamefile,line);
			numGames++;
		}
		gamefile.close();
	}
	game_Info * games = new game_Info[numGames];
	int select = 0;
	ifstream gameData("/home/student/marwoomd/Public/connections.txt");
	if(gameData.is_open()){//build array of game_Info for each game
		while(!gameData.eof()){
			getline(gameData,line);
			istringstream iss(line);
			string word;
			iss >> games[select].port;
			iss >> games[select].host;
			iss >> games[select].name;
         iss >> games[select].userName;
			if(games[select].userName != ""){
				select++;
			}
		}
	}
	gamefile.close();
	ofstream writeFile;
	writeFile.open("/home/student/marwoomd/Public/connections.txt");
	for(int i = 0; i <= select;i++){//rewrite the file, if we find a name match skip writting it.
		if(games[i].name != name){
			writeFile << games[i].port + " " + games[i].host + " " + games[i].name + " " + games[i].userName + "\n";
		}
	}
	writeFile.close();
   return true;
}
/**********************************************************************************************
   create_Game, writes availible game to the waiting game list and creates the game_Info struct
   inputs: port number, host name, game name, and username
   outputs: game_info struct
**********************************************************************************************/
game_Info create_Game(int port, string host, string name, string userName){
	ofstream writeFile;
   game_Info gameData;
   gameData.port = port;
   gameData.host = host;
   gameData.name = name;
   gameData.userName = userName;
	string sPort;//string which will contain the result
	stringstream convert; // stringstream used for the conversion
	convert << port;//add the value of Number to the characters in the stream
	sPort = convert.str();
	writeFile.open("/home/student/marwoomd/Public/connections.txt",fstream::app);
	string data = sPort + " " + host + " " + name + " " + userName + "\n";
	writeFile << data;  
	return gameData;
}	
/**********************************************************************************************
   close_Connection, close the socket connection
   input: socket identifier
   output: boolean true if successfull
**********************************************************************************************/
bool close_Connection(int sockfd){
	close(sockfd);
	return true;
}
/**********************************************************************************************
   fillarray,fills in a char array from a string
   input: string, empty char array to fill
**********************************************************************************************/
void fillarray(string value, char * array){
   array[value.size()] = 0;
	memcpy(array,value.c_str(),value.size());
}

