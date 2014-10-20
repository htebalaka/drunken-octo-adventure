/*************************************************************************
*                    board class interface definitions
*                    Michael Grant
*                    csci265
**************************************************************************/

#ifndef BOARD_H
#define BOARD_H

#include "peice.h"
#include "player.h"
#include "globalConstants.h"


class board{
   public:
      board();
      ~board(){}

      void set_up(char player_positions[]);  // put the players peices on the board
      bool winner();  //  return true if a player has won the game
      void possible_moves(peice);   // indicate to the user what the possible moves are
      bool is_valid(int,int,int,int);  //  return if the move is valid or not
      void print_board();    //  print the board as it stands
      void make_move(int, int, int, int);  // move a peice from on space to another according to how it behaves
      void print_space(int r,int c);
      void strike(peice *&, peice *&);

   private:

      peice * space[boardSize][boardSize];
      player red, blue;
      void update_side(peice *); 
      void swap(peice *&,peice *&);
      void remove_peice(peice *);
};    

#endif
          
