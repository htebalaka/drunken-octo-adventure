
#include <ncurses.h>
#include <panel.h>
#include <iostream>
#include <functional>

#include "../headers/BoardGUI_hof.h"
#include "../headers/BoardGUI.h"

// the height and width of the game-board window
#define HEIGHT 21
#define WIDTH 51

int victory() { return rand() % 2; }
int moveIsValid() { return rand() % 2; }

// returns true if the bounds are valid
bool hof_strategoBounds(int y, int x) { return y>=0 and y<10 and x>=0 and x<10; }

void BoardGUI::giveControl()
{
   int ch;
   while ((ch=getch()))
   {
      switch(ch)
      {
         case 'i': case 'j': case 'l': case 'k':
            moveCursor(ch, hof_strategoBounds);
            break;
         case 'u':
            moveCharacter(
                  std::function<bool (int,int)>([](int y,int x) { return true; }),
                  hof_strategoBounds,
                  std::function<bool (int,int)>([](int y,int x) { return true; }),
                  std::function<void (int,int,int,int)>([](int toY, int toX, int fromY, int fromX) {return;}));
            break;
         case 'q':
            endwin();
            std::exit(1);
            break;
      }
   }
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
            {
               int tmpY = cursorY;
               int tmpX = cursorX;
               if (moveCursor(ch, canMove))
               {
                  b_mvaddch(tmpY, tmpX, bottomCh); // restore old bottomCh
                  bottomCh = b_inch(cursorY, cursorX); // set new bottomCh
                  b_mvaddch(cursorY, cursorX, topCh); // overwrite with topCh
               }
               b_refresh();
               break;
            }
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

bool BoardGUI::moveCursor(chtype ch, std::function<bool (int,int)> isValidBounds)
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


bool newGameBoundCheckPlayer1(int y, int x)
{ return not (x<0 or x>9 or y<6 or y>9); }
bool newGameBoundCheckPlayer2(int y, int x)
{ return not (x<0 or x>9 or y>3 or y<0); }
std::function<bool (int,int)> newGameBoundsCheck(bool bottomPlayer)
{ return (bottomPlayer) ? newGameBoundCheckPlayer1 : newGameBoundCheckPlayer2; }

