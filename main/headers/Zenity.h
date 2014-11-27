
#ifndef ZENITY_H
#define ZENITY_H

#include <string>
#include <vector>

namespace Zenity
{
   std::string getCmdOutput(std::string);

   std::string getUserText(std::string, std::string, std::string);
   std::string getUserText(std::string, std::string, std::string, bool);

   void zout(std::string);

   std::string getTableEntry(
           std::string title,
           std::string text,
           std::vector<std::string> columnNames,
           std::vector< std::vector<std::string> > columnEntries);

   int chooseLabel(
         std::string title,
         std::string text,
         std::string column,
         std::vector< std::string > columnEntries,
         bool useZenity);

   bool getAnswer(std::string, std::string, std::string, std::string);
   bool getAnswer(std::string, std::string, std::string, std::string, bool);
}

#endif
