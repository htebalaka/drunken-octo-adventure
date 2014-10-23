#include "../headers/globalConstants.h"
#include "../headers/mysql.h"
#include <string>
#include <cstdio>
#include <stdlib.h>
#include <iostream>
using namespace std;


string pull_Board(int gameId){
	char game[10];
	sprintf(game,"%d",gameId);
	string query="SELECT board FROM Board WHERE game_id=" + string(game) + "";
	string results = query_results(query);
	
}

int create_Board(string board, int playerId){//creates board, returns board ID - HOST ONLY
	char player[10];
	sprintf(player,"%d",playerId);
	string query = "INSERT INTO Board (board,player_a) VALUES ('" + board + "','" + string(player) + "');";
	query += "\nSELECT LAST_INSERT_ID();";
	string results = query_results(query);
	if(results != ""){//it went through!
	 return atoi(results.c_str());
	}
}		
int create_Player(string name){//creates Player, returns ID
	string query = "INSERT INTO players (name) VALUES ('" + name + "');";
	query += "\nSELECT LAST_INSERT_ID();";
	string results = query_results(query);
	cout << "RESULTS: " << endl << results << endl;
	if(results != ""){
		return atoi(results.c_str());
	}
}

bool add_Player(int playerId, int boardId){//adds player 2 to game - PLAYER 2 ONLY
	char board[10];
	sprintf(board,"%d",boardId);
	char player[10];
	sprintf(player,"%d",playerId);
	string query = "UPDATE Board SET player_b="+ string(player) +" WHERE id=" + string(board) + "";
	string results = query_results(query);
	if(results == ""){
		return true;
	}else{
		return false;
	}
}



string query_results(string query){
	string action = connect+query+"";
	cout << action << endl;
	string output = getCmdOutput(action.c_str());
	return output;
}

string getCmdOutput(string cmd)
{
   string outputString;
   FILE *outpStream;
   const int MaxLineLen = 1024;
   char  outpLine[MaxLineLen];
   cmd += " 2>&1";
   outpStream = popen(cmd.c_str(), "r");
   if (outpStream) {
      while (!feof(outpStream)) {
          if (fgets(outpLine, MaxLineLen, outpStream) != NULL) {
             outputString += outpLine;
          }
      }
      pclose(outpStream);
   }
   return outputString;
}
