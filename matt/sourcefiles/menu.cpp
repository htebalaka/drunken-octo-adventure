#include <string>
#include <iostream>
#include "../headers/sockets.h"
using namespace std;

int main(){
int sockfd;
bool action = false;
do{
	string name;
	cout << "1 - Create New Game\n";
	cout << "2 - Join Existing Game\n";
	cout << "Enter Command: ";
	int command;
	cin >> command;
	switch(command){
		case 1:
			sockfd = host_Connect();
		break;
		case 2:
			sockfd = client_Connect();
		break;
		default:
			cout << "**INVALID COMMAND**";
		break;
	}
}while(!action);
}
