/*************************************************************************
*                    board class implementations
*                    Michael Grant
*                    csci265
**************************************************************************/
// comment
#include "../headers/board.h"
#include "../headers/player.h"
#include "../headers/piece.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;
board::board()
{
   for(int pos=0;pos<boardSize;pos++){

      for(int cur=0;cur<boardSize;cur++){
         space[pos][cur] = new piece;
         space[pos][cur]->rank='E';
         space[pos][cur]->color='E';
      }
   }
   for (int row=4;row<6;row++){
      for (int pos=0;pos<boardSize;pos++){
         if((pos==2)||(pos==3)||(pos==6)||(pos==7))
            space[row][pos]->rank='L';
      }
   }
}

char board::get_space_color(int y, int x){

return space[y][x]->color;

}

/***********************************************************************************************
*                        set_up implementation
*    pre-condition:  array for player_pieces must be in range of their starting position 
*    post-condition:  board contains the positions of player
***********************************************************************************************/
void board::set_up(char color, char startPositions[])
{
   int r=0;
   int c=0;
   if (color=='B')
      for (int pos=0;pos<numberOfPieces;pos++){
         space[r][c]->rank=startPositions[pos];
         space[r][c]->color='B';
         blue.place_piece(space[r][c],startPositions[pos],'B',pos-1);
         c++;
         if(c==boardSize){
            c=0;
            r++;
         }
   } 
   else if (color=='R'){
      r=6;
      for (int pos=0;pos<numberOfPieces;pos++){
         space[r][c]->rank=startPositions[pos];
         space[r][c]->color='R';
         red.place_piece(space[r][c],startPositions[pos],'R',pos-1);
         c++;
         if(c==boardSize){
            c=0;
            r++;
         }
      }
   } 
}  // put the players pieces on the board

/********************************************************************************************
*                       winner implementation
*                pre-condition: player color of the winner is sent
*                post-condition: returns true if red wins false if blue wins
********************************************************************************************/

bool board::winner(char winnerColor, bool &won)
{
//  sends who won the game to the players GUI
   won=true;  // updates the flag for game play 
   if (winnerColor=='B')
      return false;
   else
      return true;
}  

void board::possible_moves(piece){}   // indicate to the user what the possible moves are


/**********************************************************************************************
*                            is_valid implementaion
*                    pre-condition: send a space on the board which not empty
*                    post-condition: returns if the move is allowed by game rules
**********************************************************************************************/

bool board::is_valid(int rN,int cN,int r,int c)
{
   if ((space[rN][cN]->rank=='L')||(space[r][c]->rank=='L'))return false;
   switch(space[r][c]->rank){ 
      case '1':  // Marshal
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case 'S':  // Spy
      case '9':  // Miner
           
            if ((abs(rN-r)+abs(cN-c)==1)&&(space[rN][cN]->color!=space[r][c]->color))
               return true;
            else
              return false;
         break;
      case '8':  // Scout
            if(((rN==r)||(cN==c))&&(space[rN][cN]->color=='E')){
            //  if there is anything between the scout and it's destination the move is invalid
               if (r-rN<0)
                  for (int pos=r+1;pos<rN;pos++){
                     if(space[pos][c]->rank!='E')
                        return false;
                  }
               else if (r-rN>0)
                  for(int pos=r-1;pos>rN;pos--){
                     if(space[pos][c]->rank!='E')
                        return false;
                  }
              if (cN-c<0)
                  for (int pos=cN+1;pos<c;pos++){
                     if(space[r][pos]->rank!='E')
                        return false;
                  }
               else if (cN-c>0)
                  for(int pos=c-1;pos>cN;pos--){
                     if(space[r][pos]->rank!='E')
                        return false;
                  }
                  return true;
             }
             else if((abs(r-rN)+abs(c-cN)==1)&&(space[rN][cN]->color!=space[r][c]->color))
                return true;
             else 
                return false;
             break;
      case 'B':
      case 'F':
         return false;  // the bomb pieces and flag cannot move therefor any move is invalid
      default:
         return false; 
            
   }
   return true;
}  //  return if the move is valid or not

/**************************************************************************************
*                   move a piece from one space to another space
*                   pre-condition: piece is a piece located on the board at r,c co-ordinates
*                                   and has a destination on the board
*                   post condition: piece has been moved from it's original space to the destination
**************************************************************************************/
                  
void board::make_move(int rN ,int cN,int r,int c,bool& won)
{
   if (is_valid(rN,cN,r,c)){
      if (space[rN][cN]->color=='E')
         swap(space[r][c],space[rN][cN]);
      else strike(space[r][c],space[rN][cN],won);   // if the space is not empty call strike method
   } 
}  

/*******************************************************************************************
*                    strike implementation
*                   pre-condition: two piece * are sent to method
*                   post condition: the higher ranking piece is removed from board
*******************************************************************************************/

void board::strike(piece * &attacker,piece * &defender,bool &won)
{
   switch (defender->rank){
      case '1':
          if (attacker->rank=='S'){
            remove_piece(defender);
            swap(defender,attacker);
            return;
          } 
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case 'S':
      case '9':
         if(defender->rank>attacker->rank){
            remove_piece(defender);
            swap(defender,attacker);
            
         }
         else if (defender->rank==attacker->rank){
            remove_piece(defender);
            remove_piece(attacker);
         } else
            remove_piece(attacker);
      break;

      case '8':
         if ((attacker->rank=='8')&&(defender->rank=='1')){
            remove_piece(defender);
            swap(attacker,defender);
         }
         else
            remove_piece(attacker);
      break;

      case 'B':
           if(attacker->rank=='9')
              remove_piece(defender);
           else
              remove_piece(attacker);
              swap(attacker,defender);
      break;

      case 'F':
         
         swap(attacker,defender);
         winner(attacker->color,won);
      break;
      default:
         return;
      }
}

/**********************************************************************************************
*                             remove_piece implementation
*                             pre-condition:  a piece on the board needs to be removed
*                             post-condition: the piece is no longer on board
**********************************************************************************************/

void board::remove_piece(piece * victom)
{
   victom->rank='E';
   victom->color='E';
   
}
/*******************************************************************************************
*                    swap implementation
*                   pre-condition: two piece * are sent to method
*                   post condition: the two pieces are switched with each other
*******************************************************************************************/
void board::swap(piece * &x,piece * &y)
{
   piece * temp=x;
   x=y;
   y=temp;
}
/*******************************************************************************************
*                      print board implementation
*               pre-condition: board needs to be declared
*               post-condition: board is printed to the screen
*******************************************************************************************/

void board::print_board()
{
   for(int r=0;r<10;r++){
      for(int c=0;c<10;c++){
         cout<<space[r][c]->rank<<" ";
      }
      cout<<endl;
   }
   cout<<endl;
}  //  print out the board to the screen
void board::print_space(int r,int c)
{
    cout<<"space["<<r<<"]["<<c<<"]->color:"<<space[r][c]->color;
   cout<<"space["<<r<<"]["<<c<<"}->pieceNumber"<<space[r][c]->pieceNumber;
       cout<<"space["<<r<<"]["<<c<<"]->rank:"<<space[r][c]->rank<<endl;

}

/*****************************************************************************************
*                   update_piece implementation
*              pre-condition:  a piece on the board has been moved to a new location 
*                              or a piece has been removed from the board
*             post condition:  the players pieces which was effected has been updated 
*                              in their array
*****************************************************************************************/

void board::update_side(piece * current)
{
   if (current->color=='B') blue.update(current); // call update on blue
   else if (current->color=='R') red.update(current);   //  call update on red
}


/*****************************************************************************************
*                     can_pickup  implementation
*                 pre-condition: a location of a piece is sent with a char of R or B
*                 post-condition: returns true if the piece color is the same as the player 
*                                 making move
******************************************************************************************/

bool board::can_pickup(int row,int column,char colour)
{

	
   return((space[row][column])->color == colour);

}

/*****************************************************************************************
*                 theres_no_piece_at implementation
*               pre-condition:  a location is sent to function on the board
*               post-condition: returns true if there is no piece in this location
******************************************************************************************/

bool board::theres_no_piece_at(int row,int column)
{
   return (space[row][column]->rank!='E'); 
}

/*****************************************************************************************
*                 get_rank implementation
*               pre-condition:  a location is sent to function on the board with the color
*               post-condition: returns what to display on the board
******************************************************************************************/

char board::get_rank(int row,int column,char color)
{
   if (space[row][column]->color==color)
      return space[row][column]->rank;
   else if (space[row][column]->rank =='L') return 'L';
   else return '?';
}


/*****************************************************************************************
*                 get_space_color implementation
*               pre-condition:  a location is sent to function on the board 
*               post-condition: returns the color of the piece located there
*                               
******************************************************************************************/


