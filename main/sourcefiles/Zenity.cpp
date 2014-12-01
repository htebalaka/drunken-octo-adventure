
#include <string>
#include <cstdio>
#include <vector>
#include <iostream>

#include "../headers/Zenity.h"

using namespace std;

void Zenity::zout(string message)
{
   getCmdOutput("zenity --info --text \"" + message + "\"");
}

// prompts the user for text, where the default is /def/, the prompt message
// states /text/, and the title for the message box is /title/
string Zenity::getUserText(string title, string text, string def)
{
    return getCmdOutput(
            "zenity --entry "
            "--title=\"" + title + "\" "
            "--height=240 --width=360 "
            "--text=\"" + text + "\" "
            "--entry-text=\"" + def + "\"");
}
// same as above, but optionally falls back to cout
string Zenity::getUserText(string title, string text, string def, bool useZenity)
{
   if (useZenity)
   {
      return getUserText(title, text, def);
   }
   else
   {
      string in;
      std::cout << text << " ";
      std::cin >> in;
      return in;
   }
}

// prompts the user to select an entry in a table with the given title and
// text. the column names should be given in /columns/, while the table
// contents should be given in /entries/. entries should contain vectors with
// columns.size() entries each, and any strings in those vectors should be escaped
string Zenity::getTableEntry(string title, string text, vector<string> columns, vector <vector<string> > entries)
{
    string columnconcat = "";
    for (auto i : columns)
    {  columnconcat += "--column=\"" + i + "\" "; }

    string entriesconcat = "";
    for (auto i : entries)
    {
        for (auto e : i)
        {   entriesconcat += e + " "; }
    }

    return getCmdOutput(
            "zenity --list "
            "--title=\"" + title + "\" "
            "--text=\"" + text + "\" "
            "--radiolist --editable --print-column=ALL "
            + columnconcat + " " + entriesconcat);
}

int Zenity::chooseLabel(string title, string text, string column, vector<string> columnEntries, bool useZenity)
{
   if (useZenity)
   {
      std::vector< std::vector<string> > table;
      for (auto i : columnEntries)
      {
         table.push_back({"FALSE", "\""+i+"\""});
      }
      table[0][0] = "TRUE";
      string select = Zenity::getTableEntry(title, text, {"Select", column}, table);
      for (int i=0; i<=columnEntries.size();i++)
      {
         if (columnEntries[i]+"\n" == select)
         {
            return i;
         }
      }
      return -1;
   }
   else
   {
      std::cout << text << std::endl;
      for (int i = 0; i < columnEntries.size(); ++i)
      {
         std::cout << i << ": " << columnEntries[i] << std::endl;
      }
      int userInput;
      while (1)
      {
         if (std::cin >> userInput)
         {
            if (0 <= userInput and userInput < columnEntries.size())
            {
               return userInput;
            }
         }
         std::cout << "Invalid input" << std::endl;
      }
   }
}

// prompts the user with a yes or no question, with the given /title/, /text/,
// and /yes/ or /no/ buttons.
bool Zenity::getAnswer(string title, string text, string ok, string no)
{
    return "y\n" == getCmdOutput(
            "if $(zenity --question "
            "--title=\"" + title + "\" "
            "--text=\"" + text + "\" "
            "--ok-label=\"" + ok + "\" "
            "--cancel-label=\"" + no + "\"); then echo y; else echo n;fi");
}

// same as above, but optionally reads from standard in"y";
bool Zenity::getAnswer(string title, string text, string ok, string no, bool useZenity)
{
   if (useZenity)
   {
      return getAnswer(title, text, ok, no);
   }
   else
   {
      std::cout << text << " (Y/N)? ";
      string answer;
      cin >> answer;
      if (answer=="Y")
      {
         return true;
      }
      else if (answer=="F")
      {
         return false;
      }
      else
      {
         return Zenity::getAnswer(title, text, ok, no, useZenity);
      }
   }
}

// runs the specified command, capturing the ouput it produces and returning
// it as a string. taken from the lab 9 cppZen.cpp source
string Zenity::getCmdOutput(string cmd)
{
    // string to store the final results in
    string outputString;

    // stream to capture the command output
    FILE *outpStream;

    // maximum line length when capturing
    //    command output
    const int MaxLineLen = 128;

    // storage for one line of captured output
    char  outpLine[MaxLineLen];

    // redirect the standard output from thecommand 
    cmd += " 2>&1";

    // run the command, piping the output to our stream
    outpStream = popen(cmd.c_str(), "r");
    if (outpStream) {
        // capture the output, one line at a time,
        //    appending it to our output string
        while
            (!feof(outpStream))
            {
                if
                    (fgets(outpLine,
                           MaxLineLen,
                           outpStream)
                     !=
                     NULL)
                    {
                        outputString
                            +=
                            outpLine;
                    }
            }
        pclose(outpStream);
    }
    return outputString;
}
