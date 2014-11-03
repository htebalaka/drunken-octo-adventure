
#include <ncurses.h>
#include <panel.h>
#include <iostream>
#include <functional>
#include <vector>

#include "../headers/GUI_Globals.h"
#include "../headers/BoardGUI_hof.h"
#include "../headers/BoardGUI.h"

// the height and width of the game-board window
#define HEIGHT 21
#define WIDTH 51

using namespace std;
using namespace GUI_Globals;

void BoardGUI::wait_for_player(
      function<bool (int,int)> pickupPredicate,
      function<bool (int,int,int,int)> placementPredicate,
      function<void (int,int,int,int)> execTurn)
{
   int ch;
   while ((ch=getch()))
   {
      switch(ch)
      {
         case 'i': case 'j': case 'l': case 'k':
            move_cursor(ch, boundsCheck(REG_GAME));
            break;
         case 'u':
            if (move_piece(
                     pickupPredicate,
                     boundsCheck(REG_GAME),
                     placementPredicate,
                     execTurn))
            {
               // the piece was moved and the turn is finished
               // need to redraw the board here
               return;
            }
            else
            {
               // the piece wasn't moved and the user still needs to
               // finish their turn
               break;
            }
         case 'q':
            exit_gui(1);
            break;
      }
   }
}

void BoardGUI::refresh_board(
      function<bool (int y, int x)> isEmpty,
      function<bool (int y, int x)> isRed,
      function<char (int y, int x)> getChar)
{
   for (int y=0; y<10; ++y)
   {
      for (int x=0; x<10; ++x)
      {
         if (isEmpty(y,x))
         {
            // if the place is empty draw a blan
            b_mvaddch(y,x,' ');
         }
         else
         {
            // otherwise get the character that should go there and its
            // color and print it to that location
            chtype color = isRed(y,x) ? RED : BLUE;
            chtype ch = getChar(y,x);
            b_mvaddch(y,x, ch | color);
         }
      }
   }
   // refresh the gui
   b_refresh();
}

vector< vector<char> > BoardGUI::new_game(bool isBottomPlayer)
{
   empty_grid();

   // initialize a 4x10 board with empty characters
   vector< vector<chtype> > startRegion (4, vector<chtype> (10, ' '));
   vector<chtype> pieces {'B','B','B','B','B','B','F','S','9','9','9','9','9','9','9','9','8','8','8','8','8','7','7','7','7','6','6','6','6','5','5','5','5','4','4','4','3','3','2','1'};

   // add color to the vector of characters
   chtype color = (isBottomPlayer) ? RED : BLUE;
   for (int i=0; i<pieces.size(); ++i)
   {
      pieces[i] = pieces[i] | color;
   }

   chtype bottomCh = b_inch(cursorY, cursorX);
   int index = 0;

   // loop until all pieces have been placed
   chtype ch;
   while ((ch=getch()) and index < pieces.size())
   {
      // find the y index of the cursor relative to the region, 6 is the
      // index where the bottom region begins, otherwise it is 0. this is
      // useful for a number of the cases
      int regionY = cursorY - (isBottomPlayer ? 6 : 0);
      // find the piece that is currently selected, so that we can have the
      // piece move with the cursor
      chtype topCh = pieces[index];
      
      switch(ch)
      {
         case 'j': case 'i': case 'l': case 'k':
            move_cursor_while_holding(ch, bottomCh, topCh, boundsCheck(
                     (isBottomPlayer) ? NEWGAME_BOTTOM : NEWGAME_TOP));
            break;
         case 'u':
            if (startRegion[regionY][cursorX] != ' ')
            {
               // if a piece is already placed we swap it with the current
               // piece
               chtype tmp = startRegion[regionY][cursorX];
               startRegion[regionY][cursorX] = pieces[index];
               pieces[index] = tmp;
            }
            else
            {
               // otherwise, we place the piece and increment the index
               startRegion[regionY][cursorX] = pieces[index];
               ++index;
            }
            // then keep track of which character is being hidden, and draw
            // the currently selected character under the cursor
            bottomCh = startRegion[regionY][cursorX];
            topCh = pieces[index];
            b_mvaddch(cursorY, cursorX, topCh);
            b_refresh();
            break;
         case 'q':
            exit_gui(1);
            break;
      }
   }

   // convert the vector of chtypes to a vector of chars and return
   vector< vector<char> > rvalue (4, vector<char> (10, ' '));
   for (int i=0; i<4; ++i)
   {
      for (int j=0; j<10; ++j)
      {
         rvalue[i][j] = startRegion[i][j];
      }
   }
   return rvalue;
}

// attempts to pickup a piece at a location and move to another location
// keeps track of the piece that is picked up in order to make it move with
// the cursor, as well as its original location to make the original piece
// blink as a visual indication to what piece is selected. returns when a
// piece is no longer being held, either because it was moved or because the
// player opted not to move it
//
// pre-post conditions:
// no pieces are blinking, and the piece under the cursor is the same as the
// piece on the board
bool BoardGUI::move_piece(
      std::function<bool (int,int)> canPickup,
      std::function<bool (int,int)> canMove,
      std::function<bool (int,int,int,int)> canPlace,
      std::function<void (int,int,int,int)> update)
{
   // don't continue if we can't pickup the piece
   if (not canPickup(cursorY, cursorX)) return false;

   // make 'picked up' piece blink
   int origY = cursorY;
   int origX = cursorX;
   b_blink(origY, origX, true);
   // make 'picked up' piece move with cursor
   chtype topCh = b_inch(cursorY, cursorX);
   chtype bottomCh = b_inch(cursorY, cursorX);

   chtype ch;
   while ((ch=getch()))
   {
      switch(ch)
      {
         case 'j': case 'k': case 'l': case 'i':
            // if the player hits a movement key just move the piece
            move_cursor_while_holding(ch, bottomCh, topCh, canMove);
            break;
         case 'u':
            // if the player attempts to place the piece then...
            if (canPlace(cursorY, cursorX, origY, origY))
            {
               // execute the move and make the original piece blank
               update(cursorY, cursorX, origY, origX);
               b_mvaddch(origY, origX, ' ');
               b_blink(origY, origX, false);
               return true;
            } else if (origX==cursorX and origY==cursorY)
            {
               // if they attempt to move a piece to its original location
               // just return false to indicate no move took place, returning
               // control to the caller
               b_blink(origY, origX, false);
               return false;
            }
            break;
         case 'q':
            exit_gui(1);
            break;
      }
   }
   exit_gui(1);
   return 0;
}

/* pre-post conditions
 * every piece except that under the cursor is correct
 * the cursor is within the board region
 */
void BoardGUI::move_cursor_while_holding(chtype arrow, chtype& bottomCh, chtype topCh,
      function<bool (int,int)> movementPredicate)
{  
   int tmpY = cursorY;
   int tmpX = cursorX;
   if (move_cursor(arrow, movementPredicate))
   {
      // if we did in fact move we must restore the old bottomCh, set a new
      // bottomCh, and then place the topCh over the new bottomCh
      b_mvaddch(tmpY, tmpX, bottomCh);
      bottomCh = b_inch(cursorY, cursorX);
      b_mvaddch(cursorY, cursorX, topCh);
   }
   b_refresh();
}

/* pre-post conditions
 * the cursor is within the board region
 */
bool BoardGUI::move_cursor(chtype ch, std::function<bool (int,int)> isValidBounds)
{
   int oldY = cursorY;
   int oldX = cursorX;
   switch(ch)
   {
      case 'j':
         --cursorX;
         break;
      case 'k':
         ++cursorY;
         break;
      case 'l':
         ++cursorX;
         break;
      case 'i':
         --cursorY;
         break;
      default:
         exit_gui_loudly(1, "BoardGUI::move_cursor case failure");
   }
   if (not isValidBounds(cursorY, cursorX))
   {
      // restore the cursor to the previous value
      cursorX = oldX;
      cursorY = oldY;
      b_refresh();
      return false;
   }
   else
   {
      b_refresh();
      return true;
   }
}

BoardGUI::BoardGUI(int starty, int startx)
{
   cursorX = 5;
   cursorY = 5;

   win = newwin(HEIGHT, WIDTH, starty, startx);
   box(win, 0, 0);
   keypad(win, true); // allows us to track KEY_ENTER
   wrefresh(win);
}

void BoardGUI::empty_grid()
{
   mvwaddstr(win, 0, 0,  "-----------------------------------------");
   mvwaddstr(win, 1, 0,  "|   |   |   |   |   |   |   |   |   |   |");
   mvwaddstr(win, 2, 0,  "|---|---|---|---|---|---|---|---|---|---|");
   mvwaddstr(win, 3, 0,  "|   |   |   |   |   |   |   |   |   |   |");
   mvwaddstr(win, 4, 0,  "|---|---|---|---|---|---|---|---|---|---|");
   mvwaddstr(win, 5, 0,  "|   |   |   |   |   |   |   |   |   |   |");
   mvwaddstr(win, 6, 0,  "|---|---|---|---|---|---|---|---|---|---|");
   mvwaddstr(win, 7, 0,  "|   |   |   |   |   |   |   |   |   |   |");
   mvwaddstr(win, 8, 0,  "|---|---|-------|---|---|-------|---|---|");
   mvwaddstr(win, 9, 0,  "|   |   |       |   |   |       |   |   |");
   mvwaddstr(win, 10, 0, "|---|---|       |---|---|       |---|---|");
   mvwaddstr(win, 11, 0, "|   |   |       |   |   |       |   |   |");
   mvwaddstr(win, 12, 0, "|---|---|-------|---|---|-------|---|---|");
   mvwaddstr(win, 13, 0, "|   |   |   |   |   |   |   |   |   |   |");
   mvwaddstr(win, 14, 0, "|---|---|---|---|---|---|---|---|---|---|");
   mvwaddstr(win, 15, 0, "|   |   |   |   |   |   |   |   |   |   |");
   mvwaddstr(win, 16, 0, "|---|---|---|---|---|---|---|---|---|---|");
   mvwaddstr(win, 17, 0, "|   |   |   |   |   |   |   |   |   |   |");
   mvwaddstr(win, 18, 0, "|---|---|---|---|---|---|---|---|---|---|");
   mvwaddstr(win, 19, 0, "|   |   |   |   |   |   |   |   |   |   |");
   mvwaddstr(win, 20, 0, "-----------------------------------------");
   wrefresh(win);
}

