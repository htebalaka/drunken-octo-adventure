#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <iostream>
#include<fstream>
#include<sstream>
#include <string>
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
#include"../headers/sockets.h"
#include "../headers/globalConstants.h"
using namespace std;



void *get_in_addr(struct sockaddr *sa)//get socket address IPv4 or IPv6
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}
int client_Connect(){
	game_Info game;
	game = get_Game();
	string sport ;
	stringstream out;
	out << game.port;
	sport = out.str();
	int sockfd;  
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo((game.host).c_str(), sport.c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);

	freeaddrinfo(servinfo); // all done with this structure

	char recm[MAXDATASIZE];
	recv(sockfd, recm, MAXDATASIZE, 0 );
	int rec = atoi(recm);
	if(rec == 1){
		send(sockfd, "2", MAXDATASIZE, 0);
		cout << "Starting game...\n";
		return sockfd;
	}
	return 0;
}
	

int host_Connect(){
	int choose = rand() % 10;
	char s[INET6_ADDRSTRLEN];
	int port = defaultPorts[choose];//picks random port
	string sport ;
	stringstream out;
	out << port;
	sport = out.str();
	cout << "Please Enter a Name for your Game:\n";
	string name;
	cin >> name;

	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	int rv;
        char hostname[128];
        gethostname(hostname, 127);
	create_Game(port,hostname,name);      
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, sport.c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
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
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

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
		printf("server: got connection from %s\n", s);

		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
			send(new_fd, "1", MAXDATASIZE, 0);
			char recm[MAXDATASIZE];
			recv(new_fd, recm, MAXDATASIZE,0);
			int rec = atoi(recm);
			if(rec == 2){
				cout << "Starting Game!\n";
				return new_fd;
			}
		}
	}
	return 0;	
}

game_Info get_Game(){
	string line;
	ifstream gamefile("/home/student/marwoomd/Public/connections.txt");
	int numGames = 0;
	if(gamefile.is_open()){
		while(!gamefile.eof()){
			getline(gamefile,line);
			numGames++;
		}
		gamefile.close();
	}
	game_Info * games = new game_Info[numGames];
	int select = 0;
	ifstream gameData("/home/student/marwoomd/Public/connections.txt");
	if(gameData.is_open()){
		while(!gameData.eof()){
			getline(gameData,line);
			istringstream iss(line);
			string word;
			iss >> games[select].port;
			iss >> games[select].host;
			iss >> games[select].name;
			select++;
		}
	}
	gamefile.close();
	cout << "Availible Games:\n";
	cout << "Games:" << numGames << endl;
	for(int i=0;i < (numGames-1);i++){
		cout << i << " : " << games[i].name << endl;
	}
	cout << "Enter Game: ";
	int dGame;
	do{
	cin >> dGame;
	if((dGame < 0) || (dGame > (numGames-2))){
		cout << "**ERROR Invalid Game Selection!\nPlease Enter Game: ";
	}
	}while((dGame < 0) || (dGame > (numGames-1)));
	return games[dGame];
}

bool create_Game(int port, string host, string name){
	ofstream writeFile;
	string sPort;//string which will contain the result
	stringstream convert; // stringstream used for the conversion
	convert << port;//add the value of Number to the characters in the stream
	sPort = convert.str();
	writeFile.open("/home/student/marwoomd/Public/connections.txt",fstream::app);
		string data = sPort + " " + host + " " + name + "\n";
		writeFile << data;
		return true;
}	



bool close_Connection(int sockfd){
	close(sockfd);
	return true;
}



