#include <iostream>
#include <fstream>
#include "../../headers/sockets.h"

int main(){

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
			gameData = host_Connect();				
			if(gameData.sockfd){
					gameData.playerType = 'B';
					action = true;
					turn = true;
				}else{
					cerr << "Could Not Connect Socket\n";
				}
			break;
			case 2:
				gameData = client_Connect();
				if(gameData.sockfd){
					gameData.playerType = 'R';
					action = true;
				}else{
					cerr << "Could Not Connect Socket\n";
				}
			break;
			default:
				cout << "**INVALID COMMAND**";
			break;
		}
	 
}while(!action);

cout << "---------- " << gameData.name << " ---------------\n";
cout << "Username: " << gameData.userName << endl;
cout << "Opponent: " << gameData.opponent << endl;
}
