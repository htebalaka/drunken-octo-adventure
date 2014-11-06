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
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include<sstream>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include<string>
using namespace std;

struct game_Info{
	string name;
	int port;
	string address;
	string host;
};

const int maxLength = 256;
const int defaultPorts[] = {4000,4001,4002,3922,4004,4005,4006,4007,4008,4009,4010};//availible ports for the games operation.
const string gameFile= "connections.txt";

#define BACKLOG 10	 // how many pending connections queue will hold

/*
get_Game displays list of availible games to join, then prompts the user to select which game to join. Games are Managed by gameFile which is declared at the top of this file.
*/

game_Info get_Game();

string recv_f(int sockfd);
bool create_Game(int port, string host, string name);	//adds a new game to the gameFile	

int client_Connect(); //connect client(player 2) to their chosen game, returns socket ID

int host_Connect(); //creates the game and waits for player 2.
bool close_Connection(int sockfd);
