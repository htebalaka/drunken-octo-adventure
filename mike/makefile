
CXX=g++
flags=-Wall






testBoard:  headers/globalConstants.h headers/peice.h headers/player.h headers/board.h  objectfiles/player.o objectfiles/board.o objectfiles/game.o
	echo making testBoard
	$(CXX) $(flags) objectfiles/player.o objectfiles/board.o objectfiles/game.o -o game

objectfiles/player.o: headers/globalConstants.h headers/peice.h headers/player.h sourcefiles/player.cpp
	echo making player.o
	$(CXX) $(flags) sourcefiles/player.cpp -c -o objectfiles/player.o

objectfiles/board.o:  headers/globalConstants.h headers/peice.h headers/player.h headers/board.h sourcefiles/board.cpp
	echo making board.o
	$(CXX) $(flags) sourcefiles/board.cpp -c -o objectfiles/board.o

objectfiles/game.o: headers/globalConstants.h headers/peice.h headers/player.h headers/board.h  sourcefiles/game.cpp
	echo making game.o
	$(CXX) $(flags) sourcefiles/game.cpp -c -o objectfiles/game.o

