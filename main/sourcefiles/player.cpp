/*************************************************************************
*                    player class implementations
*                    Michael Grant
*                    csci265
**************************************************************************/
#include "../headers/peice.h"
#include "../headers/player.h"
#include <iostream>
#include <string>
using namespace std;

player::player()
{
   //for(int pos=0;pos<numberOfPeices;pos++){
     // cout<<playerPeices[pos]<<endl;
  // }

}


/***********************************************************************************
*                   place a peice on the board 
*                pre-condition: current must be a peice pointer and r,c in range
*                                between 0 and the const boardSize
*                post-condition:  current will be placed on the board at location r,c
*                                  with the rank of R
****************************************************************************************/

void player::place_peice(peice * current,char R ,char C,int pos)
{
   
   current->rank=R;
   current->color=C; 
   current->peiceNumber=pos;
   playerPeices[pos]=current;

}

void player::print_peice(int pos)
{
cout<<"current->color:"<<playerPeices[pos]->color;
cout<<" current->peiceNumber:"<<playerPeices[pos]->peiceNumber;
cout<<" current->rank:"<<playerPeices[pos]->rank<<endl;
}

/********************************************************************************
*                       remove method implementation
*                       pre-condition: a peice lost a challenge
*                       post-condition: the peice has been removed from the array
**********************************************************************************/

void player::remove(int pos)
{
}
/********************************************************************************
*                       update method implementation
*                       pre-condition: a peice for a side has been changed somehow
*                       post-condition: the playerPeices array has been updated
**********************************************************************************/

void player::update(peice * current)
{
   playerPeices[current->peiceNumber]=current;
}


void player::print_playerPeices()
{
   for (int pos=0;pos<40;pos++){
cout<<"pos:"<<pos<<" ";
print_peice(pos);
   }
}
