string pull_Board(int gameId){
	string query="SELECT board FROM Board WHERE game_id=\""+gameId+"\"";
	string results = query_results(query);
	
}

int create_Board(string board, int playerId){//creates board, returns board ID - HOST ONLY
	string query = "INSERT INTO Board(board,player_a) OUTPUT INSERTED.id VALUES ("+board+","+playerID+")";
	string results = query_results(query);
	if(results != ""){//it went through!
	 return results;
}
		
int create_Player(string name){//creates Player, returns ID
	string query = "INSERT INTO players(name) OUTPUT INSERTED.id VALUES ("+name+")";
	string results = query_results(query);
	if(results != ""){
		return results;
	}
}

bool add_Player(int playerID, int boardID){//adds player 2 to game - PLAYER 2 ONLY
	string query = "UPDATE Board SET player_b="+playerID+" WHERE id="+boardID;
	string results = query_results(query);
	if(results == ""){
		return true;
	}else{
		return false;
	}
}



string query_results(string query){
	string action = connects+query+"'";
	string output = getCmdOutput(action.c_str());
	return output;
}

string getCmdOutput(string cmd)
{
   string outputString;
   FILE *outpStream;
   const int MaxLineLen = 1024;
   char  outpLine[MaxLineLen];
   cmd += " 2>&1";
   outpStream = popen(cmd.c_str(), "r");
   if (outpStream) {
      while (!feof(outpStream)) {
          if (fgets(outpLine, MaxLineLen, outpStream) != NULL) {
             outputString += outpLine;
          }
      }
      pclose(outpStream);
   }
   return outputString;
}
