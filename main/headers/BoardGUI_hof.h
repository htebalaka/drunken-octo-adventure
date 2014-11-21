
#ifndef BOARD_GUI_HOF_H
#define BOARD_GUI_HOF_H

#include <functional>
#include <vector>

namespace BoardGUI_hof
{
   enum GAME_MODE { NEWGAME_TOP=-1, NEWGAME_BOTTOM=1, REG_GAME=0 };
   std::function<bool (int,int)> boundsCheck(GAME_MODE);
   char* flattenVec(std::vector< std::vector<char> >, bool);

   std::pair< std::vector< std::vector<char> >, bool> un_flatten_vec(char*);
   bool flatten_vec_identity(std::vector< std::vector<char> >, bool);
}

#endif


