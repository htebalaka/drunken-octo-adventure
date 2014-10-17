
#ifndef BOARD_GUI_H
#define BOARD_GUI_H

#include <ncurses.h>
#include <panel.h>
#include <iostream>
#include <functional>

#define HEIGHT 21
#define WIDTH 51

using namespace std;

class BoardGUI
{
   private:
      // the window the game board is drawn on 
      WINDOW* win;
      // the location of the current cursor, as indexed into the available
      // spaces on the board, not all characters in the window. functions
      // that increment/decrement these do not have to account for the
      // number of spaces between valid board locations
      int cursorX, cursorY;

      // interface to ncurses functions, which safely convert board-based
      // indices to character-based indices. these functions follow ncurses
      // naming conventions. these functions might modify the ncurses
      // cursor, but they do not modify cursor{X/Y}, and calling wrefresh
      // will reset the ncurses cursor to the correct location
      void b_mvaddch(int y, int x, chtype ch)
      { mvwaddch(win, 1+2*y, 2+4*x, ch); }
      void b_move(int y, int x) { wmove(win, 1+2*y, 2+4*x); }
      chtype b_inch(int y, int x) { return mvwinch(win, 1+2*y, 2+4*x); }
      void b_refresh()
      {
         b_move(cursorY, cursorX);

         wrefresh(win);
      }

      void b_blink(int y, int x, bool on)
      {
         chtype ch = b_inch(y,x) & A_CHARTEXT;
         chtype color = b_inch(y,x) & A_COLOR;
         if (on) { b_mvaddch(y, x, ch|color|WA_BLINK); }
         else { b_mvaddch(y, x, ch | color); }
      }

      // moves the cursor up, left, right, or down, ensuring it remains in
      // the region specified by the predicate function. returns 1 if the
      // cursor was successfully moved, 0 otherwise. asserts that the
      // chtype is a movement key.
      bool moveCursor(chtype direction, std::function<bool (int, int)> bounds);

      // attempts to pickup the character under the cursor, allowing the
      // user to move it around and place it in a new location. takes
      // predicate functions specifying:
      // - whether a particular location can be picked up. i'd like to
      //   move this logic outside of the function, though keeping it
      //   internal ensures that it is safe to call moveCursor on invalid
      //   pieces
      // - whether the user can move the cursor to a particular region
      // - whether the user can place the character at a particular spot
      // - an update function taking ((toY,toX),(fromY,fromX))
      void moveCharacter(
            std::function<bool (int,int)> canPickup,
            std::function<bool (int,int)> canMove,
            std::function<bool (int,int)> canPlace,
            std::function<void (int,int,int,int)> update);
            //std::function<void (std::pair<int,int>,std::pair<int,int>)> update);

   public:
      BoardGUI(int starty, int startx);

      // loops through the game board array to draw an up to date GUI
      void refreshBoard() { b_refresh(); }
      // gives control of the board to the user
      void giveControl();
      // allows the player to place all of their pieces at the start
      void newGame(bool bottomPlayer);
      // clears the board
      void emptyGrid();
};

#endif
