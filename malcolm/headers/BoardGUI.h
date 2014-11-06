
#ifndef BOARD_GUI_H
#define BOARD_GUI_H

#include <ncurses.h>
#include <panel.h>
#include <iostream>
#include <functional>
#include <vector>

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
      {  mvwaddch(win, 1+2*y, 2+4*x, ch); }
      void b_mvaddstr(int y, int x, const char* str)
      {  mvwaddstr(win, 1+2*y, 2+4*x, str); }
      
      void b_move(int y, int x)
      {  wmove(win, 1+2*y, 2+4*x); }
      chtype b_inch(int y, int x)
      {  return mvwinch(win, 1+2*y, 2+4*x); }
      void b_refresh()
      {  b_move(cursorY, cursorX);
         wrefresh(win);
      }

      void b_blink(int y, int x, bool on)
      {  chtype ch = b_inch(y,x) & A_CHARTEXT;
         chtype color = b_inch(y,x) & A_COLOR;
         if (on) { b_mvaddch(y, x, ch|color|WA_BLINK); }
         else { b_mvaddch(y, x, ch | color); }
      }


      // moves the cursor up/left/right/down, while "holding" the character
      // topCh, displaying topCh instead of the bottomCh hidden underneath.
      // when moving the character that is being hidden is replaced back with
      // bottomCh. ensures that the user cannot move outside of the bounds
      // specified by the movementPredicate. assumes the direction is a
      // movement key.
      void move_cursor_while_holding(
            chtype direction, 
            chtype& bottomCh, 
            chtype topCh, 
            function<bool (int,int)> movementPredicate);

      // moves the cursor up/left/right/down, ensuring it remains in the
      // region specified by the predicate function. returns true if the
      // cursor was successfully moved, 0 otherwise. asserts that the
      // direction is a movement key.
      bool move_cursor(chtype direction, function<bool (int, int)> bounds);

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
      // unlike the other move_* functions move_piece allows you to move
      // multiple spaces, whereas the others only move a single space.
      // move_piece returns when the piece is put down, either because it
      // was moved and the turn is finished (returning true) or because the
      // user decided to move a different piece (returning false)
      bool move_piece(
            function<bool (int,int)> canPickup,
            function<bool (int,int)> canMove,
            function<bool (int,int,int,int)> canPlace,
            function<void (int,int,int,int)> update);

   public:
      BoardGUI(int starty, int startx);

      // gives control to the player to let them set up their pieces at the
      // start of the game, returning a 2d vector of the pieces that they
      // placed
      vector< vector<char> > new_game(bool isBottomPlayer);

      // gives control to the player to let them make a move
      // waits for the current player to move, giving control to the user
      // until they finish their turn. takes functions to be executed to tell
      // if a piece can be picked up or moved
      void wait_for_player(
            function<bool (int Y, int X)> pickupPredicate,
            function<bool (int newY, int newX, int oldY, int oldX)> placementPredicate,
            function<void (int newY, int newX, int oldY, int oldX)> execTurn
            );

      // loops through the game board array to draw an up to date GUI. takes
      // function arguments which are used to query what to draw at each 
      // square. isRed and getChar may be undefined if the location is empty
      void refresh_board(
            function<bool (int y, int x)> isEmpty,
            function<bool (int y, int x)> isRed,
            function<char (int y, int x)> getChar);

      // clears the board
      void empty_grid();
};

#endif
