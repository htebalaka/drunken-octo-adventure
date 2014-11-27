
#include "../headers/BoardGUI_hof.h"
#include <string>
using namespace std;

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

// takes a vector of vectors, and flattens it into a single char*, with a
// leading 'R'|'B' to indicate the players color.
// Precondition: The input vector must be 10*40
// Laws:
// id = reverse . flattenVec(v, b) where
// reverse :: Char* -> (Vector (Vector Char), Bool)
void BoardGUI_hof::flattenVec(std::vector< std::vector<char> > vec, bool isBottomPlayer, std::string &positions)
{
   int i = 0;
   for (auto vec2 : vec)
   {
      for (auto e : vec2)
      {
         if (i==41)
         {
            return;
         }
         else
         {
            positions += e;
				
         }
      }
   }
}

// helper test function used by flatten_vec_identity, should be the inverse
// of flattenVec
/*pair< vector< vector<char> >, bool> BoardGUI_hof::un_flatten_vec(char* chs)
{
    vector<char> r1, r2, r3, r4;
    r1.resize(10, '_');
    r2.resize(10, '_');
    r3.resize(10, '_');
    r4.resize(10, '_');
    vector< vector<char> > rvalue {r1, r2, r3, r4};

    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<10; ++j)
        {
            rvalue[i][j] = chs[10*i+j+1];
        }
    }

    bool isRedPlayer = 'R'==chs[0];

    return make_pair(rvalue, isRedPlayer);
}

// test function for flattenVec
bool BoardGUI_hof::flatten_vec_test(vector< vector<char> > vec, bool isRedPlayer)
{
    auto p = un_flatten_vec( flattenVec(vec, isRedPlayer) );
    auto vec_1 = p.first;
    auto isRedPlayer_1 = p.second;

    return vec_1==vec and isRedPlayer_1==isRedPlayer;
}*/
