#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <cmath>

#include"../headers/sockets.h"
using namespace std;

struct sockaddr_in server;
struct sockaddr_storage their_addr;
struct in_addr ipv4addr;
struct hostent *hp;



int client_Connect(){
	game_Info game;
	game = get_Game();
int sockfd;
hp = gethostbyname((game.host).c_str());
char **alias;
alias = hp->h_aliases;
while(*alias != NULL)
{
cout << alias << endl;
alias++;
}
switch(hp->h_addrtype)
{
case AF_INET:
cout << "IP ver 4 \n";
break;
case AF_INET6:
cout << "IP ver 6 \n";
break;
}
char buff[INET_ADDRSTRLEN];
sockaddr_in addr;
addr.sin_addr = *(struct in_addr *)hp->h_addr_list[0];
addr.sin_family = hp->h_addrtype;
inet_ntop(AF_INET,&(addr.sin_addr.s_addr),buff,sizeof(buff));
server.sin_port = htons(game.port);
sockfd = socket(AF_INET,SOCK_STREAM,0);
string message = "Hello Creator!\n";
connect(sockfd, (const sockaddr *)&server, sizeof(server));
send(sockfd, (char *)message.c_str(), strlen((char *)message.c_str()), 0);
cout << "connected to: " << buff << endl;
return 0;




	/*int sockfd, numbytes;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	cout << "Connecting to " << game.host << " on port " << game.port << endl;
	inet_pton(AF_INET, game.host.c_str(), &ipv4addr);
	hp = gethostbyname((game.host).c_str());
   bcopy(hp->h_addr, &(server.sin_addr.s_addr), hp->h_length);
   server.sin_port = htons(game.port);	
	if(connect(sockfd, (const sockaddr *)&server, sizeof(server)) < 0){
		cout << "failed to connect!" << endl;
		
	}else{
		cout << "Connected!" << endl;
		char buf[256];
		int numbytes = send(sockfd, "hello creator!", 20,0);
		if(numbytes == -1){
			cout << "CLient ERROR";
		}
		return sockfd;
	}*/
	
}
	

int host_Connect(){
	int sockfd, newsockfd;
	int choose = rand() % 10;
	int port = defaultPorts[choose];//picks random port
	cout << "port:" << port << endl;
	char hostname[128];
	socklen_t clilen;
   gethostname(hostname, 127);
	sockfd = socket(AF_INET,SOCK_STREAM,0);	
	server.sin_port = htons(33222);	
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) != -1){
		cout << "Please Enter a Name for your Game:\n";
		string name;
		cin >> name;
		if(create_Game(port,hostname,name)){
			cout << "Waiting for a Connection...\n";
			listen(sockfd,5);
			clilen = sizeof their_addr;
			newsockfd = accept(sockfd, (struct sockaddr *)&their_addr, &clilen);
			if (newsockfd < 0){
  				cout << "ERROR Connecting to Client!\n";
			}else{
				cout << "CONNECTED!\n";
				char buf[256];
				int numbytes = recv(newsockfd, buf, 256-1, 0);
				cout << "message: " << buf << endl;				
				return newsockfd;
			}
		}else{
			cout << "Failed to Create the Game!\n";
			return -1;
		}
	}else{
		cout << "Failed to Bind Socket!\n";
		return -1;
	}	

}

bool close_Connection(int sockfd){
	close(sockfd);
	return true;
}



void *get_in_addr(struct sockaddr *sa)//get socket address IPv4 or IPv6
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

