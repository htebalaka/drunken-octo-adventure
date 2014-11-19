CXX=g++
flags=-lpanel -lncurses -std=c++11

GLOBALS=../headers/globalConstants.h

SERVER_CPP=../sourcefiles/server.cpp
SERVER_H=../headers/sockets.h
TESTSERVER_CPP=sourcefiles/testserver.cpp


testserver: ${TESTSERVER_CPP} ${SERVER_CPP} ${SERVER_H}
	${CXX} ${flags} ${TESTSERVER_CPP} ${SERVER_CPP} -o testserver




