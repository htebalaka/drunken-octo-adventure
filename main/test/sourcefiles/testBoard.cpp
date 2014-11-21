#include"../../headers/piece.h"
#include"../../headers/player.h"
#include"../../headers/board.h"
#include<iostream>
#include<fstream>

using namespace std;

char getCommand();

void test_set_up(player testPlayer,board testBoard);  // put the players pieces on the board
void test_winner(board testBoard);  //  return true if a player has won the game
void test_possible_moves(piece);   // indicate to the user what the possible moves are
void test_is_valid(board testBoard);  //  return if the move is valid or not
void test_make_move(board testBoard);  // move a piece from on space to another according to how it behaves
void test_strike(board testBoard);  // determines the winner of a strike
void test_can_pickup(board testBoard);  // determines if a player can pick up a piece 
void test_theres_no_piece_at(board testBoard);  // returns if there is a piece at this location


int main()
{
   player testPlayer;
   board  testBoard;
   char cmd='z';
   while (cmd!='Q'){   
      cmd=getCommand();

      switch (cmd){
         case 'S':  // setup
                    test_set_up(testPlayer,testBoard);
                    break;
         case 'W':  // winner
                    test_winner(testBoard);
                    break;
         case 'P':  // possible_moves - not yet implemented
                    break;
         case 'I':  // is_valid
                    test_is_valid(testBoard);
                    break;
         case 'M':  // make_move
                    test_make_move(testBoard);
                    break;
         case 'T':  // strike
                    test_strike(testBoard);
                    break;
         case 'C':  // can_pickup
                    test_can_pickup(testBoard);
                    break;
         case 'N':  // theres_no_piece_at
                    test_theres_no_piece_at(testBoard);
         default:  // continue loop intill Q is entered
                   break;
      }
   }
   return 0;
}

char getCommand()
{
   cout<<"Enter Q to quit\n";
   cout<<"      S to test set_up\n";
   cout<<"      W to test winner\n";
   cout<<"      I to test is_valid\n";
   cout<<"      M to test make_move\n";
   cout<<"      T to test strike\n";
   cout<<"      C to test can_pickup\n";
   cout<<"      N to test theres_no_piece_at\n";
   char cmd=getchar();
   char garbage=getchar();
   return toupper(cmd);
}

void test_set_up(player testPlayer,board testBoard)
{
   cout<<"testing set_up:\n";
   cout<<"Enter testplayer piece array (41 chars starting with B or R):\n";
   char positions[41];
   for (int pos=0;pos<numberOfPieces+1;pos++)
      cin>>positions[pos];
   testBoard.set_up(positions);
   testBoard.print_board();
}  


void test_winner(board testBoard)
{
   cout<<"Testing winner:\n";
   char win='R';
   cin>>win;
   if(testBoard.winner(win))
      cout<<"Red side won\n";
   else
      cout<<"Blue side won\n";
}  


void test_possible_moves(piece)
{
// still in production
}  
void test_is_valid(board testBoard)
{
   int r,c,nr,nc;
   cout<<"Testing is_valid:\n";
   cout<<"Enter coordinates for the piece you want to move:\n";
   cin>>r>>c;
   cout<<"Enter coordinates for the new location of the piece:\n";
   cin>>nr>>nc;
   if(testBoard.is_valid(nr,nc,r,c))
      cout<<"Move is valid\n";
   else
      cout<<"Move is invalid\n";
} 
void test_make_move(board testBoard)
{
   cout<<"testing make_move:\n";
   int r,c,nr,nc;
   cout<<"Enter coordinates for the piece you want to move:\n";
   cin>>r>>c;
   cout<<"Enter coordinates for the new location of the piece:\n";
   cin>>nr>>nc;
   testBoard.make_move(nr,nc,r,c) ;
   testBoard.print_board();  
}  


void test_strike(board testBoard)
{
   cout<<"Testing strike\n";
   piece* one=new piece;
   piece* two=new piece;
   cout<<"Enter the rank for the attacker: \n";
   cin>>one->rank;
   cout<<"Enter the rank for the deffender:\n";
   testBoard.strike(one,two);
   if (one->rank=='E')
      cout<<"defender loses\n";
   else if(two->rank=='E')
      cout<<"attacker loses\n";
   else if (one->rank=='E'&& two->rank=='E')
      cout<<"Ranks were the same both out of game\n";   
}

  
void test_can_pickup(board testBoard)
{
   cout<<"Testing can_pickup:\n";
   cout<<"Enter coordinate of piece and color of side R/B";
   int r,c;
   char color;
   cin>>r>>c>>color;
   if(testBoard.can_pickup(r,c,color))
      cout<<"Piece can be picked up\n";
   else
      cout<<"Piece cannot be picked up\n";
} 


void test_theres_no_piece_at(board testBoard)
{
   int r,c;
   cout<<"Testing theres_no_piece_at:\n";
   cout<<"Enter a coordinate you want to check\n";
   cin>>r>>c;
   if(testBoard.theres_no_piece_at(r,c))
      cout<<"There's no piece at that location\n";
   else
      cout<<"There's a piece at that location\n";
}  

