
#ifndef ZENITY_H
#define ZENITY_H

#include <string>
#include <vector>

namespace Zenity
{
   std::string getCmdOutput(std::string);
   std::string getUserText(std::string, std::string, std::string);
   std::string getTableEntry(std::string, std::string, std::vector<std::string>, std::vector<std::string>);
   std::string getAnswer(std::string, std::string, std::string, std::string);
   std::string getCmdOutput(std::string);
}

#endif
