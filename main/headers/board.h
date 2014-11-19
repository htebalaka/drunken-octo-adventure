/*************************************************************************
*                    board class interface definitions
*                    Michael Grant
*                    csci265
**************************************************************************/

#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "player.h"
#include "globalConstants.h"


class board{
   public:
      board();
      ~board(){}

      void set_up(char player_positions[]);  // put the players pieces on the board
      bool winner(char );  //  return true if a player has won the game
      void possible_moves(piece);   // indicate to the user what the possible moves are
      bool is_valid(int,int,int,int);  //  return if the move is valid or not
      void make_move(int, int, int, int);  // move a piece from on space to another according to how it behaves
      void strike(piece *&, piece *&);  // determines the winner of a strike
      bool can_pickup(int,int,char);  // determines if a player can pick up a piece 
      bool theres_no_piece_at(int,int);  // returns if there is a piece at this location

      void print_board();    //  print the board as it stands 
      void print_space(int r,int c);  // debugging method
   private:

      piece * space[boardSize][boardSize];
      player red, blue;
      void update_side(piece *); 
      void swap(piece *&,piece *&);
      void remove_piece(piece *);
};    

#endif
          
