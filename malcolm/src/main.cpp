
#include <ncurses.h>
#include <panel.h>
#include <iostream>

#include "../headers/BoardGUI.h"
#include "../headers/GUI_Globals.h"

using namespace GUI_Globals;

int main()
{
   init_gui();

   BoardGUI gui = smart_init_board();
   gui.new_game(true);
   exit_gui(0);
   return 0;
}

