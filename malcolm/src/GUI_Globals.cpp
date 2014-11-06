
#include <ncurses.h>
#include <string>
#include <iostream>
#include "../headers/GUI_Globals.h"

bool GUI_Globals::GUI_IS_ON = false;

// sets up global variables necessary to use ncurses
void GUI_Globals::init_gui()
{
   setlocale(LC_ALL, "");
   initscr();
   start_color();
   raw();
   keypad(stdscr, TRUE);
   noecho();
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
{  if (GUI_Globals::GUI_IS_ON)
   {  endwin(); }
   std::exit(errCode);
}

void GUI_Globals::exit_gui_loudly(int errCode, std::string format)
{  if (GUI_Globals::GUI_IS_ON)
   {  endwin(); }
   std::cout << format;
   std::exit(errCode);
}

void GUI_Globals::assert(bool condition, std::string format)
{  if (!condition)
   { GUI_Globals::exit_gui_loudly(1, format); }
}
