
#include <string>
#include <iostream>
#include <vector>
#include "../../headers/Zenity.h"

using namespace std;
using namespace Zenity;

int main()
{
    cout << getUserText("title", "text", "default") << endl;

    cout << getTableEntry("title", "text", {"column1","column2"},
            {{"1","\"game 1\""},{"2","\"game 2\""}}) << endl;

    cout << getAnswer("title","question","yes","no") << endl;

    return 0;
}
