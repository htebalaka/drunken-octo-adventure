
#include <ncurses.h>
#include <iostream>

#include "../headers/BoardGUI.h"
#include "../headers/MainGUI.h"

MainGUI::MainGUI()
{
   initscr();
   start_color();
   raw();
   keypad(stdscr, TRUE);
   noecho();
   MEVENT event;
   mousemask(ALL_MOUSE_EVENTS, NULL);
   if (has_colors() == FALSE)
   {
      endwin();
      printf("Your terminal doesn't support colors\n");
   }

   init_pair(5, COLOR_RED, -1);
   init_pair(6, COLOR_BLUE, -1);

   int starty = (LINES-HEIGHT)/2;
   int startx = (COLS-WIDTH)/2;
   BoardGUI gui(starty, startx);
   gui.emptyGrid();
   gui.new_game(true);
   gui.giveControl();
}

MainGUI::~MainGUI()
{
   endwin();
}
