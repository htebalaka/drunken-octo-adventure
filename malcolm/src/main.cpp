
#include <ncurses.h>
#include <panel.h>
#include <iostream>
#include <locale.h>

#include "../headers/BoardGUI.h"
#include "../headers/GUI_Globals.h"

using namespace GUI_Globals;

int main()
{
   init_gui();

   int starty = (LINES-HEIGHT)/2;
   int startx = (COLS-WIDTH)/2;
   BoardGUI gui(starty, startx);
   gui.new_game(true);
   
   exit_gui(0);
}

