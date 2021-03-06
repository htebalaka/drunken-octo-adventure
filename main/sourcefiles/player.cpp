/*************************************************************************
*                    player class implementations
*                    Michael Grant
*                    csci265
**************************************************************************/
#include "../headers/piece.h"
#include "../headers/player.h"
#include <iostream>
#include <string>
using namespace std;

player::player()
{
   //for(int pos=0;pos<numberOfpieces;pos++){
     // cout<<playerpieces[pos]<<endl;
  // }

}


/***********************************************************************************
*                   place a piece on the board 
*                pre-condition: current must be a piece pointer and r,c in range
*                                between 0 and the const boardSize
*                post-condition:  current will be placed on the board at location r,c
*                                  with the rank of R
****************************************************************************************/

void player::place_piece(piece * current,char R ,char C,int pos)
{
   
   current->rank=R;
   current->color=C; 
   current->pieceNumber=pos;
   playerPieces[pos]=current;

}

void player::print_piece(int pos)
{
   cout<<"current->color:"<<playerPieces[pos]->color;
   cout<<" current->pieceNumber:"<<playerPieces[pos]->pieceNumber;
   cout<<" current->rank:"<<playerPieces[pos]->rank<<endl;
}

/********************************************************************************
*                       update method implementation
*                       pre-condition: a piece for a side has been changed somehow
*                       post-condition: the playerpieces array has been updated
**********************************************************************************/

void player::update(piece * current)
{
   playerPieces[current->pieceNumber]=current;
}


void player::print_playerPieces()
{
   for (int pos=0;pos<40;pos++){
      cout<<"pos:"<<pos<<" ";
      print_piece(pos);
   }
}
