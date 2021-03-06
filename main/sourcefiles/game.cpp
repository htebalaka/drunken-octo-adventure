/*************************************************************************
*                    game play source file
*                    Michael Grant
*                    csci265
**************************************************************************/

#include "../headers/board.h"
#include "../headers/player.h"
#include "../headers/piece.h"
#include "../headers/globalConstants.h"
#include "../headers/sockets.h"
#include "../headers/BoardGUI.h"
#include "../headers/GUI_Globals.h"
#include "../headers/BoardGUI_hof.h"
#include "../headers/Zenity.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;
using namespace GUI_Globals;
using namespace BoardGUI_hof;

/**********************************************************************************************
*                       function prototypes
**********************************************************************************************/
void cout_board(board& game, game_Info& gameData)
{
   string no_piece, get_color, get_rank;
   no_piece = "theres_no_piece_at\n";
   get_color = "get_space_color\n";
   get_rank = "get_rank\n";

   for (int i=0; i<100; ++i)
   {
      int y = i / 10;
      int x = i % 10;

      no_piece += to_string(game.theres_no_piece_at(y,x));

      if (game.get_space_color(y,x)=='R')
      {   get_color += "R"; }
      else if (game.get_space_color(y,x)=='B')
      {   get_color += "B";}
      else if (game.get_space_color(y,x)=='E')
      {   get_color += "E";}
      else
      {  get_color += "?"; }
      
      get_rank += to_string(game.get_rank(y,x,gameData.playerType));
      if ((i+1)%10==0)
      {
         no_piece += "\n";
         get_color += "\n";
         get_rank += "\n";
      }
   }
   no_piece += "\n";
   get_color += "\n";
   get_rank += "\n";

   Zenity::getAnswer("cout_board", no_piece+get_color+get_rank,"yes","no");
}
// intergrates the GUI and game logic to draw the currently up to date board
void draw_board(BoardGUI& gui, board& game, game_Info& gameData)
{

   gui.refresh_board(
         [&](int y, int x) -> bool
         {
         // this gets executed to test whether a location is empty
         return !game.theres_no_piece_at(y,x);
         },
         [&](int y, int x) -> bool
         {
         // this gets executed to see if a non-empty location is red
         return (game.get_space_color(y,x) == 'R') ? true : false;
         },
         [&](int y, int x) -> char
         {
         // this gets executed to see what character should go in what
         // location
         return game.get_rank(y, x, gameData.playerType);
         });
}

int main()
{
bool stop = false;
bool turn = false;
string winner = "";
/**********************************************************************************************
*                       create a session and find a challenger
                        input required: none;
                        output: gameData//defined in sockets.h
                                    includes: port, host, name//of game , userName, opponent,sockfd
                                 playerType//R or B
                                 boardData//board Data of other player
                                 positions//board Data of you!
                               
**********************************************************************************************/
game_Info gameData;//All relevent game data stored here.

bool action = false;
	do{
      int command = Zenity::chooseLabel(
            "Stratego!",
            "",
            "Choices",
            {"Create a new game","Join existing game"},
            true)+1;
		//string name;
		//cout << "1 - Create New Game\n";
		//cout << "2 - Join Existing Game\n";
		//cout << "Enter Command: ";
		//int command;
		//cin >> command;
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
				if(gameData.sockfd && (gameData.sockfd != -1)){
					gameData.playerType = 'R';
					action = true;
				}else{
					break;
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

/**********************************************************************************************
*                       enter game session
**********************************************************************************************/
        // while the players want to play maintain a connection
/**********************************************************************************************
*                      create a new game
**********************************************************************************************/
	using namespace GUI_Globals;
	init_gui();
	BoardGUI gui = smart_init_board();
	Zenity::zout("Set your Players!");
   bool isBottomPlayer = gameData.playerType == 'R';
	auto starting_board = gui.new_game(isBottomPlayer);
	string rpositions = "";
	string ropponentBoard = "";
	BoardGUI_hof::flattenVec(starting_board, isBottomPlayer, rpositions);//set your pieces
	sync_Board(rpositions,gameData, ropponentBoard);
	char positions[40];//your pieces
	char opponentBoard[40];//opponents pieces
	for(int i =0;i<=40;i++){
		positions[i] = rpositions[i];
		opponentBoard[i] = ropponentBoard[i];
	}  
	board game;//game board
  	game.set_up(gameData.playerType, positions);//setup your pieces on the board
	game.set_up(((gameData.playerType == 'R') ? 'B' : 'R'), opponentBoard);//set opponents pieces on the board
   draw_board(gui, game, gameData);
   using namespace Zenity;
/**********************************************************************************************
*                       begin play
*                  player who creates the game is blue and gets to go first
**********************************************************************************************/
	while (!stop){//while no winner
		draw_board(gui, game, gameData);
		if(turn == true){//its your turn, make a move
			Zenity::zout("Its Your Turn!");
        	gui.wait_for_player(
            [&](int y, int x) -> bool
            {
               // this gets executed to check whether we can pickup a piece     
               return game.can_pickup(y, x, gameData.playerType);
            },
            [&](int toY, int toX, int fromY, int fromX) -> bool
            {
               // this gets executed to check whether we can move a piece
               return (game.is_valid(toY, toX, fromY, fromX));
            },
            [&](int toY, int toX, int fromY, int fromX) -> void
            {
               // this gets executed when we make a move
	   			std::string moveData;
					moveData += to_string(toY);
					moveData += ' ';
					moveData += to_string(toX);
					moveData += ' ';
					moveData += to_string(fromY);
					moveData += ' ';
					moveData += to_string(fromX);
					if(send_Move(moveData,gameData,turn)){//if you were allowed to move, and it sent, then update		
                  game.make_move(toY, toX, fromY, fromX, stop);
					}else{
						stop = true;
						exit_gui_loudly("IT WASNT YOUR MOVE!!!\n");		
					}
             }
				);
			//wait for other player move here
		}else if(turn == false){
			std::string otherPlayerMove = get_Move(gameData, turn);	
			istringstream moves(otherPlayerMove);	
			int toY;
			int toX;
			int fromY;
			int fromX;
			moves >> toY;
			moves >> toX;
			moves >> fromY;
			moves >> fromX;
			game.make_move(toY, toX, fromY, fromX, stop);
      }

   }//stop loop   
/**********************************************************************************************
*                         enter shut down procedures                      
**********************************************************************************************/
   GUI_Globals::exit_gui_quietly();
   return 0;
}



