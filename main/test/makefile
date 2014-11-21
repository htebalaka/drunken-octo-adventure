CXX=g++
flags=-lpanel -lncurses -std=c++11

FLAGS= -Wall 
GLOBALS=../headers/globalConstants.h

SERVER_CPP=../sourcefiles/server.cpp
SERVER_H=../headers/sockets.h
TESTSERVER_CPP=sourcefiles/testserver.cpp

PLAYER_H=../headers/player.h
BOARD_H=../headers/board.h
PLAYER_CPP=../sourcefiles/player.cpp
BOARD_CPP=../sourcefiles/board.cpp

makeall: testserver testPlayer  testBoard
	echo "Making all teststubs"

testserver: ${TESTSERVER_CPP} ${SERVER_CPP} ${SERVER_H}
	${CXX} ${flags} ${TESTSERVER_CPP} ${SERVER_CPP} -o testserver

testPlayer: $(GLOBALS) $(PLAYER_H) $(PLAYER_CPP) sourcefiles/testPlayer.cpp
	echo "Making testPlayer"
	$(CXX) $(flags) $(PLAYER_CPP) testPlayer.cpp -o testPlayer

testBoard: $(GLOBALS) $(PLAYER_H) $(BOARD_H) $(PLAYER_CPP) $(BOARD_CPP) sourcefiles/testBoard.cpp
	echo "Making testBoard"
	$(CXX) $(FLAGS) $(PLAYER_CPP) $(BOARD_CPP) sourcefiles/testBoard.cpp -o testBoard

