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

int main()
{
cout<<"entering the main program\n";
   char positions[numberOfPeices+1];  // add one for the player color at hte beginning of the array
cout<<"making the game board\n";
   board game1;
cout<<"printing the game board\n";
   game1.print_board();
   char data;
   fstream redPlayer;
   fstream bluePlayer;
   redPlayer.open("testData/redPlayer.txt");
  
   bluePlayer.open("testData/bluePlayer.txt");
   for (int pos=0;pos<=numberOfPeices+1;pos++){
      redPlayer>>data;
      positions[pos]=data;
   } 
   player red;
   redPlayer.close();
   game1.set_up(positions);
   for (int pos=0;pos<=numberOfPeices+1;pos++){
      bluePlayer>>data;
      positions[pos]=data;
   }
   player blue;
   bluePlayer.close();
   game1.set_up(positions);
   game1.print_board();
int x=0;
   while(x!=100){
      int a,b,c;
      cout<<"enter the coordinates for your peice:";
      cin>>a>>b;
      cout<<"enter the coordinates of where you wnat it moved:";
      cin>>c>>x;
   

   game1.make_move(a,b,c,x);
   game1.print_board();
   }
   return 0;
}
