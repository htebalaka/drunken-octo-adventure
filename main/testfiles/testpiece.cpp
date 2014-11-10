/**********************************************************************************************
*                            test driver for piece class methods
**********************************************************************************************/

#include"peice.h"
#include<iostream>
#include<fstream>
#include<instream>

using namespace std;
/**********************************************************************************************
*                         function prototypes
**********************************************************************************************/
void test_place_piece(player &testPlayer,piece playerPiece);
void test_update_piece();

int main()
{
   player testPlayer;
   piece* playerPiece;

   test_place_piece(testPlayer,playerPiece);
   test_update(testPlayer,piece playerPiece);
   return 0;
}

void test_place_piece()
{
   instream testdata;
   char color,rank;
   int pieceNumber;
   testdata.open("placePieceTest.txt");
   while (!eof){
      testdata>>color;
      for (int pos=0;pos<numberOfPieces;pos++){
         testdata>>rank;
         playerPiece = new piece;
         testPlayer.place_piece(playerPiece,rank,color,pos);
      }
   testPlayer.print_playerPieces();
   }
}

void test_update()
{
   player testPlayer;
   instream testData;
   piece* currentPiece;
   char rank;
   int pieceNumber;
   testData.open("updatePieceTest.txt");
   while(!eof){
      testData>>
   }
}
