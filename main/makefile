
CXX=g++
flags=-lpanel -lncurses -std=c++11

GUI_GLOBALS_H=headers/GUI_Globals.h
GUI_GLOBALS_CPP=sourcefiles/GUI_Globals.cpp
GUI_GLOBALS_O=objectfiles/GUI_Globals.o

ZENITY_H=headers/Zenity.h
ZENITY_CPP=sourcefiles/Zenity.cpp
ZENITY_O=objectfiles/Zenity.o

BOARDGUI_HOF_H=headers/BoardGUI_hof.h

BOARDGUI_H=headers/BoardGUI.h
BOARDGUI_CPP=sourcefiles/BoardGUI.cpp
BOARDGUI_O=objectfiles/BoardGUI.o

.PHONY: clean


testBoard:  headers/globalConstants.h headers/piece.h headers/player.h headers/board.h  objectfiles/player.o objectfiles/board.o objectfiles/game.o objectfiles/server.o $(BOARDGUI_O) $(GUI_GLOBALS_O)
	echo making testBoard
	$(CXX) $(flags) objectfiles/player.o objectfiles/board.o objectfiles/game.o objectfiles/server.o $(BOARDGUI_O) $(GUI_GLOBALS_O) -o testboard

objectfiles/player.o: headers/globalConstants.h headers/piece.h headers/player.h sourcefiles/player.cpp
	echo making player.o
	$(CXX) $(flags) sourcefiles/player.cpp -c -o objectfiles/player.o

objectfiles/board.o:  headers/globalConstants.h headers/piece.h headers/player.h headers/board.h sourcefiles/board.cpp
	echo making board.o
	$(CXX) $(flags) sourcefiles/board.cpp -c -o objectfiles/board.o

objectfiles/game.o: headers/globalConstants.h headers/piece.h headers/player.h headers/board.h  sourcefiles/game.cpp headers/sockets.h
	echo making game.o
	$(CXX) $(flags) sourcefiles/game.cpp -c -o objectfiles/game.o

objectfiles/server.o:  sourcefiles/server.cpp headers/sockets.h headers/globalConstants.h
	echo making server.o
	$(CXX) $(flags) sourcefiles/server.cpp -c -o objectfiles/server.o

testPlayer:  headers/player.h headers/piece.h objectfiles/player.o testfiles/testPlayer.cpp
	echo making testPlayer driver
	$(CXX) $(flags) objectfiles/player.o testfiles/testPlayer.cpp  -o testPlayer

${BOARDGUI_O}: ${BOARDGUI_CPP} ${BOARDGUI_H} ${BOARDGUI_HOF_H}
	@echo "Creating ${BOARDGUI_O}"
	${CXX} -c ${BOARDGUI_CPP} ${flags} -o ${BOARDGUI_O}
${GUI_GLOBALS_O}: ${GUI_GLOBALS_CPP} ${GUI_GLOBALS_H} ${BOARDGUI_H}
	@echo "Creating ${GUI_GLOBALS_O}"
	${CXX} -c ${GUI_GLOBALS_CPP} ${flags} -o ${GUI_GLOBALS_O}



clean:
	rm objectfiles/*.o


