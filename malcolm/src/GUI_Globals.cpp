
#include <ncurses.h>
#include <string>
#include <iostream>

#include "../headers/BoardGUI.h"
#include "../headers/GUI_Globals.h"

BoardGUI GUI_Globals::smart_init_board()
{
   int starty=(LINES-HEIGHT)/2;
   int startx=(COLS-WIDTH)/2;
   return BoardGUI(starty, startx);
}

bool GUI_Globals::GUI_IS_ON = false;

// sets up global variables necessary to use ncurses
void GUI_Globals::init_gui()
{
   //setlocale(LC_ALL, "");
   initscr();
   start_color();
   raw();
   keypad(stdscr, TRUE);
   noecho();
   MEVENT event;
   mousemask(ALL_MOUSE_EVENTS, NULL);
   if (has_colors() == FALSE)
   {  endwin();
      printf("Your terminal doesn't support colors\n");
      exit(1);
   }
   init_pair(1, COLOR_RED, COLOR_BLACK);
   init_pair(2, COLOR_BLUE, COLOR_BLACK);
   GUI_Globals::GUI_IS_ON = true;
}


void GUI_Globals::exit_gui(int errCode)
{  endwin();
   std::exit(errCode);
}

void GUI_Globals::exit_gui_loudly(std::string format)
{  endwin();
   std::cout << format;
   std::exit(1);
}

void GUI_Globals::assert(bool condition, std::string format)
{  if (!condition)
   { GUI_Globals::exit_gui_loudly(format); }
}
