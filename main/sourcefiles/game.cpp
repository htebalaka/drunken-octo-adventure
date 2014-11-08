/*************************************************************************
*                    game play source file
*                    Michael Grant
*                    csci265
**************************************************************************/

#include "../headers/board.h"
#include "../headers/player.h"
#include "../headers/peice.h"
#include "../headers/globalConstants.h"
#include "../headers/sockets.h"
#include <iostream>
#include <fstream>
#include "stdio.h"

using namespace std;

/**********************************************************************************************
*                       function prototypes
**********************************************************************************************/

void won(bool whoWon);

void fillarray(string value, char * array);

int main()
{
bool stop = false;
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
char playerType;//B=host,R=client

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
					playerType = 'B';
					action = true;
				}else{
					cerr << "Could Not Connect Socket\n";
				}
			break;
			case 2:
				gameData = client_Connect();
				if(gameData.sockfd){
					playerType = 'R';
					action = true;
				}else{
					cerr << "Could Not Connect Socket\n";
				}
			break;
			default:
				cout << "**INVALID COMMAND**";
			break;
		}
      string initialize;
      initialize += playerType + " ";
      string positions;
      positions = "1 2 3 4 5 6 7 8 9 1 2 3 4 5 6 7 8 9 1";//for debugging only. remove later
/*********************************************************************************************
   Board setup here, initialize current player positions
    and put into positions string as formatted above
*********************************************************************************************/
      initialize += positions;
      char buffer[MAXDATASIZE];
      fillarray(initialize, buffer);
      string boardData;//storage for board information received from other player.

      switch(playerType){//sockets are a 1 way street, cant send and recieve at the same time...

         case 'B'://host  always sends first
            send(gameData.sockfd, buffer , MAXDATASIZE, 0);//send board data to client
            recv(gameData.sockfd, buffer , MAXDATASIZE, 0);//recieve clients board data
            boardData = buffer;
            fillarray(gameData.userName, buffer);
            send(gameData.sockfd, buffer , MAXDATASIZE, 0);//send username to client
            recv(gameData.sockfd, buffer , MAXDATASIZE, 0);//recieve opponent username
            gameData.opponent = buffer; //set opponent

         break;

         case 'R'://client recieves first
            recv(gameData.sockfd, buffer , MAXDATASIZE, 0);//recieve hosts board data
            boardData = buffer;
            send(gameData.sockfd, buffer , MAXDATASIZE, 0);//send board data to host
            recv(gameData.sockfd, buffer , MAXDATASIZE, 0);//recieve username from host
            gameData.opponent = buffer; //set opponent
            fillarray(gameData.userName, buffer);
            send(gameData.sockfd, buffer , MAXDATASIZE, 0);//send username to host
         break;

         default://something went wrong!
            cout << "ERROR, Unable to determine player type!\n";
            stop = true;//shut down, we cant continue
         break;
      }
      
	}while(!action);


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
      //  get player positions
      //  place the peices into the players piece arrays
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
      bool whoWon=true;  // indicator to tell who won the game 0 for blue  1 for red

      if (!quit){
         // all in game functions and proper game play logic in here

/**********************************************************************************************
*                       begin play
*                  player who creates the game is blue and gets to go first
**********************************************************************************************/
        

         //  check to see if the red player has won or if red has quit the game 
         //  if not get the blue players move
         // update the board - the board object will update the players piece array
         game.make_move(row,column,newRow,newColumn);
         // check to see if the blue player has won or if blue has quit the game
         // if not get the red players move 
         // update the board
         game.make_move(row,column,newRow,newColumn); 
         // if either player has won exit loop
      }  // exit game play loop
      won(whoWon);  //  send who won the game to the end of game function
   }  // exit game session loop
   
   
/**********************************************************************************************
*                         enter shut down procedures                      
**********************************************************************************************/

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

void fillarray(string value, char * array){
   for(int i=0;i<value.size();i++){
      array[i] = value[i];
   }
}

