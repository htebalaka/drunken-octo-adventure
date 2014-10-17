
#include <ncurses.h>
#include <panel.h>
#include <iostream>

#include "../headers/BoardGUI.h"

int main()
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
      return 1;
   }
   init_pair(1, COLOR_RED, -1);
   init_pair(2, COLOR_BLUE, -1);

   BoardGUI gui(20, 20);
   getch();

   int starty=(LINES-10)/2;
   int startx=(COLS-19)/2;

   gui.emptyGrid();
   while (1)
   {
      gui.giveControl();
   }

   // end curses mode
   endwin();
   return 0;
}

