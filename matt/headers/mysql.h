#ifndef MYSQL_H
#define MYSQL_H
#include <string>
using namespace std;

string pull_Board(int gameId);// fetch board from database
int create_Board(string board, int playerId);//Creates board, returns board id - HOST ONLY
int create_Player(string name);//creates Player, returns ID
bool add_Player(int playerId, int boardId);//adds player 2 to game - PLAYER 2 ONLY
string query_results(string query);//runs Query on database, returns raw results
string getCmdOutput(string cmd);//runs MYSQL commands

#endif
