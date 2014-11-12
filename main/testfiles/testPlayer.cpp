/**********************************************************************************************
*                            test driver for piece class methods
**********************************************************************************************/

#include"../headers/piece.h"
#include"../headers/player.h"
#include<iostream>
#include<fstream>

using namespace std;
/**********************************************************************************************
*                         function prototypes
**********************************************************************************************/
void test_place_piece(player testPlayer,piece* playerPiece);
void test_update(player testPlayer,piece* playerPiece);

int main()
{
   player testPlayer;
   piece* playerPiece= new piece;
   cout<<"Testing place_piece\n";
   test_place_piece(testPlayer,playerPiece);

   cout<<"Testing update\n";
   test_update(testPlayer,playerPiece);
   return 0;
}

void test_place_piece(player testPlayer,piece* playerPiece)
{
   char color,rank,cmd;
   //  get the data 
   do {
      cout<<"Enter a color: \n";
      cin>>color;
      for (int pos=0;pos<numberOfPieces;pos++){
         cout<<"Enter a rank: \n";
         cin>>rank;
         playerPiece = new piece;
         testPlayer.place_piece(playerPiece,rank,color,pos);
      }
   testPlayer.print_playerPieces();
   cout<<"Enter D to quit or anything else to continue\n";
   cin>>cmd;
   }while (cmd!='D');
}

void test_update(player testPlayer,piece* playerPiece)
{
   char rank,color,cmd;
   int pieceNumber=0;
   // fill the playerPieces array with test data
   do {
      cout<<"Enter a color: \n";
      cin>>color;
      for (int pos=0;pos<numberOfPieces;pos++){
         piece* currentPiece = new piece;
         cout<<"Enter a rank: \n";
         cin>>rank;
         testPlayer.place_piece(currentPiece,rank,color,pos);
         
      }  
      testPlayer.print_playerPieces();
      do{
         cout<<"change a piece of your choice: \n";
         cout<<"enter a piece number to change: \n";
         cin>>pieceNumber;
         playerPiece->pieceNumber=pieceNumber;
       
         cout<<"Enter a rank: \n";
         cin>>rank;
         playerPiece->rank=rank;

         cout<<"Enter a color: \n";
         cin>>color;
         playerPiece->color= color;

         
         testPlayer.update(playerPiece);
         cout<<"Enter D to quit or anything else to continue\n";
         cin>>cmd;

      }while (cmd!='D');
    
      cout<<"Enter D to quit or anything else to continue\n";
      cin>>cmd;
      
        
   } while(cmd!='D');
}
