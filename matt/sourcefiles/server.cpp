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

struct sockaddr_in server, cli_addr;
struct in_addr ipv4addr;
struct hostent *hp;



int client_Connect(){
	game_Info game;
	game = get_Game();
	int sockfd, numbytes;
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
		return sockfd;
	}
	
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
	server.sin_port = htons(port);	
	server.sin_addr.s_addr = AI_PASSIVE;	
	if (bind(sockfd, (struct sockaddr *)&server,sizeof(server)) < 0){
		cout << "Please Enter a Name for your Game:\n";
		string name;
		cin >> name;
		if(create_Game(port,hostname,name)){
			cout << "Waiting for a Connection...\n";
			listen(sockfd,5);
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
			if (newsockfd < 0){
  				cout << "ERROR Connecting to Client!\n";
			}else{
				cout << "CONNECTED!\n";
				char buf[256];
				int numbytes = recv(sockfd, buf, 256-1, 0);
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

