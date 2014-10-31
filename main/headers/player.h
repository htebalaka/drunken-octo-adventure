/*************************************************************************
*                    player class interface definitions
*                    Michael Grant
*                    csci265
**************************************************************************/




#ifndef PLAYER_H
#define PLAYER_H
#include "peice.h"
#include "globalConstants.h"

class player
{
     public:
        player();
        ~player(){}

        void place_peice(peice * ,char,char,int);  // initialize the players peices at the beginning of the game
        void update(peice *);  // update the player peice array
        void remove(int);  // remove a peice from a players side
void print_peice(int);
void print_playerPeices();
     private:
        peice * playerPeices[numberOfPeices];
};

#endif
