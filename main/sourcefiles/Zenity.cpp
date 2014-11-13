
#include <string>
#include <cstdio>
#include <vector>

#include "../headers/Zenity.h"

using namespace std;

string Zenity::getUserText(string title, string text, string def)
{
   return getCmdOutput(
         "zenity --entry "
                "--title=\"" + title + "\" "
                "--height=240 --width=360 "
                "--text=\"" + text + "\" "
                "--entry-text=\"" + def + "\"");
}

string Zenity::getTableEntry(string title, string text, vector<string> columns, vector<string> entries)
{
   string columnconcat = "";
   for (auto i : columns)
   {  columnconcat += "--column=\"" + i + "\" "; }

   string entriesconcat = "";
   for (auto i : entries)
   {  entriesconcat += i + " "; }

   return getCmdOutput(
         "zenity --list "
                "--title=\"" + title + "\" "
                "--text=\"" + text + "\" "
                + columnconcat + " " + entriesconcat);
}

string Zenity::getAnswer(string title, string text, string ok, string no)
{
   return getCmdOutput(
         "zenity --question "
                "--title=\"" + title + "\" "
                "--text=\"" + text + "\" "
                "--ok-label=\"" + ok + "\" "
                "--cancel-label=\"" + no + "\" ");
}

// run the specified command, capture the output 
//    it produces and return it as a string
// taken from the lab9 cppZen.cpp sourcecode
string getCmdOutput(string cmd)
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
