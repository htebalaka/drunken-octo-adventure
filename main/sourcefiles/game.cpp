/*************************************************************************
*                    game play source file
*                    Michael Grant
*                    csci265
**************************************************************************/

#include "../headers/board.h"
#include "../headers/player.h"
#include "../headers/peice.h"
#include "../headers/globalConstants.h"
#include <iostream>
#include <fstream>
#include "stdio.h"

using namespace std;

/**********************************************************************************************
*                       function prototypes
**********************************************************************************************/

void won(bool whoWon);

int main()
{
/**********************************************************************************************
*                       create a session and find a challenger
**********************************************************************************************/




/**********************************************************************************************
*                       enter game session
**********************************************************************************************/
   bool stop = false;
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

