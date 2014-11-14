
#include "../headers/BoardGUI_hof.h"

// returns a predicate function for the valid bounds of the GUI for:
// -1 -> top player beginning of game
// 1  -> bottom player beginning of game
// _  -> rest of game
std::function<bool (int,int)> BoardGUI_hof::boundsCheck(GAME_MODE mode)
{
   switch(mode)
   {  case NEWGAME_TOP:
      return std::function<bool (int,int)>([](int y, int x) {
            return x>=0 and x<10 and y>=0 and y<4; });
      break;
      case NEWGAME_BOTTOM:
      return std::function<bool (int,int)>([](int y, int x) {
            return x>=0 and x<10 and y>=6 and y<10; });
      break;
      default:
      return std::function<bool (int,int)>([](int y, int x) {
            return x>=0 and x<10 and y>=0 and y<10; });
      break;
   }
}

char* BoardGUI_hof::flattenVec(std::vector< std::vector<char> > vec, bool isBottomPlayer)
{
   char rvalue[41];
   rvalue[0] = isBottomPlayer ? 'R' : 'B';
   int i = 1;
   for (auto vec2 : vec)
   {
      for (auto e : vec2)
      {  
         if (i==41)
         {
            return rvalue;
         }
         else
         {
            rvalue[i] = e;
         }
      }
   }
}

