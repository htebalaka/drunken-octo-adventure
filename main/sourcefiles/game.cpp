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
#include <iostream>
#include <fstream>
#include <sstream>
#include "stdio.h"

using namespace std;
using namespace GUI_Globals;
using namespace BoardGUI_hof;

/**********************************************************************************************
*                       function prototypes
**********************************************************************************************/

void won(bool whoWon);

int main()
{
bool stop = false;
bool turn = false;
bool winner = false;
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
	using namespace GUI_Globals;
	init_gui();
	BoardGUI gui = smart_init_board();
   bool isBottomPlayer = gameData.playerType == 'R';
	auto starting_board = gui.new_game(isBottomPlayer);
	char *positions = BoardGUI_hof::flattenVec(starting_board, isBottomPlayer);
	char *opponentBoard = sync_Board(positions,gameData);
   exit_gui_quietly();
	stop = true;//for debugging only

/**********************************************************************************************
*                       enter game session
**********************************************************************************************/
   while (!stop){      // while the players want to play maintain a connection
/**********************************************************************************************
*                      create a new game
**********************************************************************************************/

      board game;  // create a board object which the game is played on
      char redPieces[41];  // declare an array to hold red's initial piece positions
      char bluePieces[41];  // declare an array to hold blue's initioal piece positions
      bool whowon;  // a boolian variabe to indicate who has won 0 for red 1 for blue
      //  get player positions
      //  place the pieces into the players piece arrays
      //  place the pieces on the board

/**********************************************************************************************
*                          main game loop
*         pre-condition: A player has created a game and a challenger has joined the game
*   post-condition: A player has won the game or either player has quit or susspended the game
**********************************************************************************************/

/**********************************************************************************************
*                       declare variables for game play
**********************************************************************************************/

      bool quit=false;  // a flag to control game play loop
      int row,column,newRow,newColumn;  // variables to pass coordinates to board object
      row=column=newRow=newColumn=0;  // initialize coordinate variables to 0
      whowon=true;  // indicator to tell who won the game 0 for blue  1 for red

      if (!quit){
         // all in game functions and proper game play logic in here

/**********************************************************************************************
*                       begin play
*                  player who creates the game is blue and gets to go first
**********************************************************************************************/
        

         if (winner){  //  check to see if the blue player has won or if red has quit the game 
            quit=true; 
            whowon=false;  // flag is false indicating red is the winner
         }
 
         //  if not get the red players move
         // update the board - the board object will update the players piece array

         // this function gets called to give control to the current player
         gui.wait_for_player(
               [gameData](int y, int x)
               {
                  // this gets executed to check whether we can pickup a piece
                  return board.can_pickup(y, x, gameData.playerType);
               },
               [](int toY, int toX, int fromY, int fromX)
               {
                  // this gets executed to check whether we can move a piece
                  return board.is_valid(toY, toX, fromY, fromX);
               },
               [gameData, turn, quit](int toY, int toX, int fromY, int fromX)
               {
                  // this gets executed when we make a move
						std::string moveData = toY + ' ' + toX + ' ' + fromY + ' ' + fromX;
						if(make_move(moveData,gameData,turn)){
                  	return board.make_move(toY, toX, fromY, fromX);
						}else{
							quit = true;
							exit_gui_quietly();
							cout << "IT WASNT YOUR MOVE!!!\n";
			
						}
               });
         // this function gets called to give control to the player across the
         // network


			//wait for other player move here
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
			board.make_move(toY, toX, fromY, fromX)

         gui.refresh_board(
               [](int y, int x)
               {
                  // this gets executed to test whether a location is empty
                  return board.theres_no_piece_at(y,x);
               },
               [](int y, int x)
               {
                  // this gets executed to see if a non-empty location is red
                  return (board.color[y][x] == 'R') ? true : false;
               },
               [](int y, int x)
               {
                  // this gets executed to see what character should go in what
                  // location
                  return (board.char[y][x]);
               });
         game.make_move(row,column,newRow,newColumn);
         // check to see if the blue player has won or if blue has quit the game
         // if not get the red players move 
         // update the board
         game.make_move(row,column,newRow,newColumn); 

         // if either player has won exit loop
      }  // exit game play loop
      won(whowon);  //  send who won the game to the end of game function
   }  // exit game session loop
   
   
/**********************************************************************************************
*                         enter shut down procedures                      
**********************************************************************************************/
   exit_gui_quietly();
   return 0;
}

/**********************************************************************************************
*                       finction implementations
**********************************************************************************************/

void won(bool whoWon)
{
   if (whoWon) cout<<"Red won!\n";
   else cout<<"Blue won!\n";
}


