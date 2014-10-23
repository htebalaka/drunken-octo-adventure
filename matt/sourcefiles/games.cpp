#include<iostream>
#include<fstream>
#include<sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include"../headers/sockets.h"
using namespace std;

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

