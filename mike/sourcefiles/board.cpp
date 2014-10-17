/*************************************************************************
*                    board class implementations
*                    Michael Grant
*                    csci265
**************************************************************************/
// comment
#include "../headers/board.h"
#include "../headers/player.h"
#include "../headers/peice.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;
board::board()
{
   for(int pos=0;pos<boardSize;pos++){

      for(int cur=0;cur<boardSize;cur++){
         space[pos][cur] = new peice;
         space[pos][cur]->rank='E';
         space[pos][cur]->color='E';
         space[pos][cur]->row=pos;
         space[pos][cur]->column=cur;
      }
   }
   for (int row=4;row<6;row++){
      for (int pos=0;pos<boardSize;pos++){
         if((pos==2)||(pos==3)||(pos==6)||(pos==7))
            space[row][pos]->rank='L';
      }
   }
}



/***********************************************************************************************
*                        set_up implementation
*    pre-condition:  array for player_peices must be in range of their starting position 
*    post-condition:  board contains the positions of player
***********************************************************************************************/
void board::set_up(char startPositions[])
{
   int r=0;
   int c=0;
   if (startPositions[0]=='B')
      for (int pos=1;pos<numberOfPeices+1;pos++){
         space[r][c]->rank=startPositions[pos];
         space[r][c]->color='B';
         blue.place_peice(space[r][c],startPositions[pos],'B',pos);
         c++;
         if(c==boardSize){
            c=0;
            r++;
         }
   } 
   else if (startPositions[0]=='R'){
      r=6;
      for (int pos=1;pos<numberOfPeices+1;pos++){
         space[r][c]->rank=startPositions[pos];
         space[r][c]->color='R';
         red.place_peice(space[r][c],startPositions[pos],'R',pos);
         c++;
         if(c==boardSize){
            c=0;
            r++;
         }
      }
   } 
}  // put the players peices on the board

/********************************************************************************************
*                       winner implementation
*                pre-condition player strikes the opponents flag
*                post-condition: player is declared winner
********************************************************************************************/

bool board::winner()
{return false;}  //  return true if a player has won the game

void board::possible_moves(peice){}   // indicate to the user what the possible moves are


/**********************************************************************************************
*                            is_valid implementaion
*                    pre-condition: send a space on the board which not empty
*                    post-condition: returns if the move is allowed by game rules
**********************************************************************************************/

bool board::is_valid(peice* current,int r,int c)
{
   if ((current->rank=='L')||(space[r][c]->rank=='L'))return false;
   switch(current->rank){ 
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
           
            if ((abs(current->row-r)+abs(current->column-c)==1)&&(current->color!=space[r][c]->color))
               return true;
            else
              return false;
         break;
      case 'S':
            if(((current->row==r)||(current->column==c))&&(space[r][c]->color=='E')){
            //  if there is anything between the scout and it's destination the move is invalid
               if (current->row-r<0)
                  for (int pos=current->row+1;pos<r;pos++){
                     if(space[pos][c]->rank!='E')
                        return false;
                  }
               else if (current->row-r>0)
                  for(int pos=r-1;pos>current->row;pos--){
                     if(space[pos][c]->rank!='E')
                        return false;
                  }
              if (current->column-c<0)
                  for (int pos=current->column+1;pos<c;pos++){
                     if(space[r][pos]->rank!='E')
                        return false;
                  }
               else if (current->column-c>0)
                  for(int pos=c-1;pos>current->column;pos--){
                     if(space[r][pos]->rank!='E')
                        return false;
                  }
                  return true;
                  return true;

             }
             else if((abs(current->row-r)+abs(current->column-c)==1)&&(current->color!=space[r][c]->color))
                return true;
             else 
                return false;
             break;
      case 'B':
      case 'F':
         return false;  // the bomb peices and flag cannot move therefor any move is invalid
      default:
         return false; 
            
   }
   return true;
}  //  return if the move is valid or not

/**************************************************************************************
*                   move a peice from one space to another space
*                   pre-condition: peice is a peice located on the board at r,c co-ordinates
*                                   and has a destination on the board
*                   post condition: peice has been moved from it's original space to the destination
**************************************************************************************/
                  
void board::make_move(int r ,int c,int rN,int cN)
{
   if (is_valid(space[r][c],rN,cN)){
      space[rN][cN]->rank=space[r][c]->rank;
      space[rN][cN]->color=space[r][c]->color;
      space[r][c]->color='E';
      space[r][c]->rank='E';
      update_side(space[rN][cN]);      
   } else cout<<"move is invalid\n";
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
    cout<<"space["<<r<<"]["<<c<<"]->row:"<<space[r][c]->row<<endl;
       cout<<"space["<<r<<"]["<<c<<"]->column:"<<space[r][c]->column<<endl;

}

/*****************************************************************************************
*                   update_peice implementation
*              pre-condition:  a peice on the board has been moved to a new location 
*                              or a peice has been removed from the board
*             post condition:  the players peices which was effected has been updated 
*                              in their array
*****************************************************************************************/

void board::update_side(peice * current)
{
   if (current->color=='B') blue.update(current); // call update on blue
   else if (current->color=='R') red.update(current);   //  call update on red
}
