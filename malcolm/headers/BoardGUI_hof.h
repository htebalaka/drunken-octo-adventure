
#ifndef BOARD_GUI_HOF_H
#define BOARD_GUI_HOF_H

#include <functional>

using namespace std;

// returns a predicate function for the valid bounds of the GUI for:
// -1 -> top player beginning of game
// 1  -> bottom player beginning of game
// _  -> rest of game
std::function<bool (int,int)> boundsCheck(int mode)
{
   switch(mode)
   {  case -1:
      return std::function<bool (int,int)>([](int y, int x) {
            return x>=0 and x<10 and y>=0 and y<4; });
      break;
      case 1:
      return std::function<bool (int,int)>([](int y, int x) {
            return x>=0 and x<10 and y>=6 and y<10; });
      break;
      default:
      return std::function<bool (int,int)>([](int y, int x) {
            return x>=0 and x<10 and y>=0 and y<10; });
      break;
   }
}

bool constTrue(int x, int y) { return true; }

#endif
