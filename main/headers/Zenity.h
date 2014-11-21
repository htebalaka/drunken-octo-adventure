
#ifndef ZENITY_H
#define ZENITY_H

#include <string>
#include <vector>

namespace Zenity
{
   std::string getCmdOutput(std::string);
   std::string getUserText(std::string, std::string, std::string);
   std::string getTableEntry(
           std::string title,
           std::string text,
           std::vector<std::string> columnNames,
           std::vector< std::vector<std::string> > columnEntries);
   std::string getAnswer(std::string, std::string, std::string, std::string);
   std::string getCmdOutput(std::string);
}

#endif
