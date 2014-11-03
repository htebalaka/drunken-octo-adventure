
#ifndef GUI_GLOBALS_H
#define GUI_GLOBALS_H

#include <ncurses.h>
#include <string>
#include <iostream>

namespace GUI_Globals
{
   // will be used for printing via ncurses/cout as applicable
   extern bool GUI_IS_ON;// = false;
   // used as constants for setting colors
   const chtype RED = COLOR_PAIR(1);
   const chtype BLUE = COLOR_PAIR(2);

   // sets up global variables necessary to use ncurses
   void init_gui();
   void exit_gui(int errCode);
   void exit_gui_loudly(int errCode, std::string format);
   void assert(bool condition, std::string format);
}

#endif

