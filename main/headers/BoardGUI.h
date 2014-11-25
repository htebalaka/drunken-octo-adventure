
#ifndef BOARD_GUI_H
#define BOARD_GUI_H

#include <ncurses.h>
#include <panel.h>
#include <iostream>
#include <functional>
#include <vector>
#include <fstream>

#define HEIGHT 21
#define WIDTH 51

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

        /* TEST SPECIFIC FUNCTIONS BELOW */

        // reads a character, either from the user or from the run_macro
        // ifstream
        chtype readch();

        // run_/record_macro either take user input from a file/write user
        // input to a file respectively, as well as write/record callback
        // results. the callback behaviour is used to provide later
        // simulation of previously done callbacks. when running a macro
        // no interaction with the user /OR/ game /OR/ networking should
        // be necessary
        std::ifstream* run_macro;
        std::ofstream* record_macro;
        // record_stdout writes the contents of the gui to a file, for later
        // comparsion. there is no run_stdout, as the generation of the
        // board in macro mode is done via the mentioned previously recorded
        // callback results. record_stdout is only called by b_refresh, but
        // the macro functions are unfortunately more involved.
        std::ofstream* record_stdout;

        // add runtime assertion tests. this should be implemented with 
        // DNDEBUG, but i've had some issues with it. the assert macro 
        // unfortunately won't work either, since there is cleanup that needs
        // to run before shutdown
        bool runtime_asserts;

        /* END TEST SPECIFIC FUNCTIONS */

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

            // if we are debugging print the contents of the screen to a
            // file
            if (record_stdout != nullptr)
            {
                for (int i=0; i<21; ++i)
                {
                    for (int j=0; j<51; ++j)
                    {
                        (*record_stdout) << mvwinch(win, i, j);
                        (*record_stdout) << std::endl;
                    }
                }
                (*record_stdout) << std::endl;
            }
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
                std::function<bool (int,int)> movementPredicate);

        // moves the cursor up/left/right/down, ensuring it remains in the
        // region specified by the predicate function. returns true if the
        // cursor was successfully moved, 0 otherwise. asserts that the
        // direction is a movement key.
        bool move_cursor(chtype direction, std::function<bool (int, int)> bounds);

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
                std::function<bool (int,int)> canPickup,
                std::function<bool (int,int)> canMove,
                std::function<bool (int,int,int,int)> canPlace,
                std::function<void (int,int,int,int)> update);

    public:
        void set_debug_flags(std::ifstream*, std::ofstream*, std::ofstream*);

        BoardGUI(int starty, int startx);

        // gives control to the player to let them set up their pieces at the
        // start of the game, returning a 2d vector of the pieces that they
        // placed
        std::vector< std::vector<char> > new_game(bool isBottomPlayer);

        // gives control to the player to let them make a move
        // waits for the current player to move, giving control to the user
        // until they finish their turn. takes functions to be executed to tell
        // if a piece can be picked up or moved
        void wait_for_player(
                std::function<bool (int Y, int X)> pickupPredicate,
                std::function<bool (int newY, int newX, int oldY, int oldX)> placementPredicate,
                std::function<void (int newY, int newX, int oldY, int oldX)> execTurn
                );

        // loops through the game board array to draw an up to date GUI. takes
        // function arguments which are used to query what to draw at each 
        // square. isRed and getChar may be undefined if the location is empty
        void refresh_board(
                std::function<bool (int y, int x)> isEmpty,
                std::function<bool (int y, int x)> isRed,
                std::function<char (int y, int x)> getChar);

        // clears the board
        void empty_grid();
};

#endif
