/*************************************************************************
*                    player class interface definitions
*                    Michael Grant
*                    csci265
**************************************************************************/




#ifndef PLAYER_H
#define PLAYER_H
#include "piece.h"
#include "globalConstants.h"

class player
{
     public:
        player();
        ~player(){}

        void place_piece(piece* ,char,char,int);  // initialize the players pieces at the beginning of the game
        void update(piece*);  // update the player piece array

/*                         testing methods                                                   */
        void print_piece(int);
        void print_playerPieces();
     private:
        piece* playerPieces[numberOfPieces];
};

#endif
