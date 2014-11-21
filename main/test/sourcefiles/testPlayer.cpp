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
void test_place_piece(player &testPlayer,piece* playerPiece);
void test_update(player testPlayer);
void fill_player(player &testPlayer);

int main()
{
   player testPlayer;
   piece* playerPiece= new piece;

   cout<<"to test place_piece enter P:\n";
   cout<<"to test update enter U\n";
   cout<<"to quit enter Q\n";
   
   char cmd='b';
   while(cmd!='Q'){
      cmd=getchar();
      cmd=toupper(cmd);
      switch (cmd){
         case 'P':  cout<<"Testing place_piece\n";
                 test_place_piece(testPlayer,playerPiece);
                 break;
         case 'U':  cout<<"Testing update\n";
                 test_place_piece(testPlayer,playerPiece);
                 test_update(testPlayer);
                 break;
      }
   }
   return 0;
}

void test_place_piece(player &testPlayer,piece* playerPiece)
{
   char cmd;
   //  get the data 
   do {
      fill_player(testPlayer);
      testPlayer.print_playerPieces();
      cout<<"Enter Q to quit or anything else to continue\n";
      cmd=getchar();
      cmd=toupper(cmd);
   }while (cmd!='Q');
}

void test_update(player testPlayer)
{
   piece* playerPiece= new piece;

   char rank,color,input,cmd;

   int pieceNumber=0;
   fill_player(testPlayer);   
      
    do{
         cout<<"enter a piece number to change: \n";
         input=getchar();
         if(isdigit(input)){
            playerPiece=testPlayer->pieceNumber;
       
            cout<<"Enter a rank: \n";
            rank=getchar();
            playerPiece->rank=rank;
  
            cout<<"Enter a color: \n";
            color=getchar();
            playerPiece->color= color;

            testPlayer.update(playerPiece);
            testPlayer.print_playerPieces();
         }
      else cout<<"input not valid:\n";
      cout<<"Enter Q to quit or anything else to continue\n";
      cmd=getchar();
      cmd=toupper(cmd);
        
   } while(cmd!='Q');
}

void fill_player(player &testPlayer)
{
   char color,rank;
   
   cout<<"Enter a color: \n";
      color=getchar();
      for (int pos=0;pos<numberOfPieces;pos++){
         cout<<"Enter a rank: \n";
         rank=getchar();
         piece* playerPiece = new piece;
         testPlayer.place_piece(playerPiece,rank,color,pos);
      }
}
