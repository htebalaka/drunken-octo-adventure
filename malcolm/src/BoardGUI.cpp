
#include <ncurses.h>
#include <panel.h>
#include <iostream>
#include <functional>
#include <vector>

#include "../headers/BoardGUI_hof.h"
#include "../headers/BoardGUI.h"

// the height and width of the game-board window
#define HEIGHT 21
#define WIDTH 51

int victory() { return rand() % 2; }
int moveIsValid() { return rand() % 2; }

void BoardGUI::giveControl()
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
            moveCharacter(
                  std::function<bool (int,int)>([](int y,int x) { return true; }), // can pickup
                  boundsCheck(REG_GAME),
                  std::function<bool (int,int)>([](int y,int x) { return true; }), // can place
                  std::function<void (int,int,int,int)>([](int toY, int toX, int fromY, int fromX) {return;})); // update
            break;
         case 'q':
            endwin();
            std::exit(1);
            break;
      }
   }
}



void BoardGUI::new_game(bool isBottomPlayer)
{
   // initialize a 4x10 board with empty characters
   vector< vector<char> > startRegion (4, vector<char> (10, ' '));
   vector<chtype> pieces {'1','1','1','1','2','2','2','2','3','3','3','3','4','4','4','4','5','5','5','5','6','6','6','6','7','7','7','7','8','8','8','8','9','9','9','9','0','0','0','0'};

   chtype bottomCh = b_inch(cursorY, cursorX);
   int index = 0;
   chtype ch;
   while ((ch=getch()) and index < pieces.size())
   {
      // find the y index of the cursor relative to the region, 6 is the
      // index where the bottom region begins, otherwise it is 0. this is
      // useful for a number of the cases
      int regionY = cursorY - ((isBottomPlayer) ? 6 : 0);
      // find the piece that is currently selected, so that we can have the
      // piece move with the cursor, will need to convert from piece adt
      chtype topCh = pieces[index];

      switch(ch)
      {
         case 'j': case 'i': case 'l': case 'k':
            move_cursor(ch, bottomCh, topCh, boundsCheck(
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

               bottomCh = startRegion[regionY][cursorX];
               topCh = pieces[index];
               b_mvaddch(cursorY, cursorX, topCh);
               b_refresh();
            }
            else
            {
               // if a piece is not already placed we place it and increment
               // the index to point to the next piece to place
               startRegion[regionY][cursorX] = pieces[index];
               ++index;

               bottomCh = startRegion[regionY][cursorX];
               b_refresh();
            }
            break;
         case 'q':
            endwin();
            std::exit(1);
            break;
      }
   }
   // this will need to return the startRegion vector when it comes to
   // interfacing with the game logic portion of code
}

void BoardGUI::moveCharacter(
      std::function<bool (int,int)> canPickup,
      std::function<bool (int,int)> canMove,
      std::function<bool (int,int)> canPlace,
      std::function<void (int,int,int,int)> update)
{
   if (not canPickup(cursorY, cursorX)) return;

   // make 'picked up' piece move with cursor
   chtype topCh = b_inch(cursorY, cursorX);
   // make 'picked up' piece blink
   int origY = cursorY;
   int origX = cursorX;
   b_blink(origY, origX, true);
   // make 'picked up' piece move with cursor
   chtype bottomCh = b_inch(cursorY, cursorX);

   // i would like to have an actual array that is being updated here,
   // which i could update (with a higher-order function) rather than make
   // changes in terms of GUI deltas. this would remove the need to keep
   // track of bottomCh, since i could just loop through the array and
   // then replace the character under the cursor with topCh and the
   // orig{Y/X} with blink afterwards.
   // this would be a little less efficient, but i think would be more
   // readable.

   chtype ch;
   while ((ch=getch()))
   {
      switch(ch)
      {
         case 'j': case 'k': case 'l': case 'i':
            move_cursor(ch, bottomCh, topCh, canMove);
            break;
         case 'u':
            if (canPlace(cursorY, cursorX))
            {
               update(cursorY, cursorX, origY, origX);
               b_blink(origY, origX, false);
               return;
            } else if (origX==cursorX and origY==cursorY)
            {
               b_blink(origY, origX, false);
               return;
            }
            break;
         case 'o':
            b_blink(origY, origX, false);
            return;
            break;
         case 'q':
            endwin();
            std::exit(1);
            break;
      }
   }
}

void BoardGUI::move_cursor(chtype arrow, chtype& bottomCh, chtype topCh,
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
         endwin();
         std::cout << "BoardGUI::moveCursor case failure: " << ch << std::endl;
         std::exit(1);
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

void BoardGUI::emptyGrid()
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
   b_mvaddch(0,0, 'x');
   b_mvaddch(0,1, 'y');
   b_mvaddch(0,2, 'h');
   wrefresh(win);
}

