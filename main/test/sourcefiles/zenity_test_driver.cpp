
include <vector>
include <iostream>

include "../../headers/Zenity.h"

using namespace std;
using namespace Zenity;

int main
{
	cout << getUserText("title", "text", "default");
	cout << getTableEntry("title", "text", {"column1","column2"},{{"1","\"entry1\""},{"2","\"entry2\""}};
	cout << getAnswer("title", "text", "yes", "no");
	return 0;	
}
