
CXX=g++
flags=-lpanel -lncurses -std=c++11

GLOBALS=headers/globalConstants.h

GUI_GLOBALS_H=headers/GUI_Globals.h
GUI_GLOBALS_CPP=sourcefiles/GUI_Globals.cpp
GUI_GLOBALS_O=objectfiles/GUI_Globals.o

ZENITY_H=headers/Zenity.h
ZENITY_CPP=sourcefiles/Zenity.cpp
ZENITY_O=objectfiles/Zenity.o

BOARDGUI_HOF_H=headers/BoardGUI_hof.h
BOARDGUI_HOF_CPP=sourcefiles/BoardGUI_hof.cpp
BOARDGUI_HOF_O=objectfiles/BoardGUI_hof.o

BOARDGUI_H=headers/BoardGUI.h
BOARDGUI_CPP=sourcefiles/BoardGUI.cpp
BOARDGUI_O=objectfiles/BoardGUI.o

SERVER_CPP=sourcefiles/server.cpp
SERVER_H=headers/sockets.h
SERVER_O=objectfiles/server.o

PIECE_H=headers/piece.h

PLAYER_CPP=sourcefiles/player.cpp
PLAYER_H=headers/player.h
PLAYER_O=objectfiles/player.o

BOARD_CPP=sourcefiles/board.cpp
BOARD_H=headers/board.h
BOARD_O=objectfiles/board.o

GUI_CPP=sourcefiles/main.cpp

GAME_CPP = sourcefiles/game.cpp
GAME_O=objectfiles/game.o


.PHONY: clean
.PHONY: gui

testBoard: ${PLAYER_O} ${BOARD_O} ${GAME_O} ${SERVER_O} $(BOARDGUI_O) $(GUI_GLOBALS_O) ${BOARDGUI_HOF_O}
	echo making testBoard
	$(CXX) $(flags) ${PLAYER_O} ${BOARD_O} ${GAME_O} ${SERVER_O} $(BOARDGUI_O) $(GUI_GLOBALS_O) ${BOARDGUI_HOF_O} -o testboard

${PLAYER_O}: ${GLOBALS} ${PIECE_H} ${PLAYER_H} ${PLAYER_CPP}
	echo making ${PLAYER_O}
	$(CXX) $(flags) ${PLAYER_CPP} -c -o ${PLAYER_O}

${BOARD_O}: ${GLOBALS} ${PIECE_H} ${PLAYER_H} ${BOARD_H} ${BOARD_CPP}
	echo making ${BOARD_O}
	$(CXX) $(flags) ${BOARD_CPP} -c -o ${BOARD_O}

${GAME_O}: ${GLOBALS} ${PIECE_H} ${PLAYER_H} ${BOARD_H} ${GAME_CPP} ${SERVER_H}
	echo making ${GAME_O}
	$(CXX) $(flags) ${GAME_CPP} -c -o ${GAME_O}

${SERVER_O}: ${SERVER_CPP} ${SERVER_H} ${GLOBALS}
	echo making ${SERVER_O}
	$(CXX) $(flags) ${SERVER_CPP} -c -o ${SERVER_O}

testPlayer: ${PLAYER_O}
	echo making testPlayer driver
	$(CXX) $(flags) ${PLAYER_O} -o testPlayer

${BOARDGUI_O}: ${BOARDGUI_CPP} ${BOARDGUI_H} ${BOARDGUI_HOF_H}
	@echo "Creating ${BOARDGUI_O}"
	${CXX} -c ${BOARDGUI_CPP} ${flags} -o ${BOARDGUI_O}

${BOARDGUI_HOF_O}: ${BOARDGUI_HOF_CPP} ${BOARDGUI_HOF_H}
	@echo "Creating ${BOARDGUI_HOF_H}"
	${CXX} -c ${BOARDGUI_HOF_CPP} ${flags} -o ${BOARDGUI_HOF_O}

${GUI_GLOBALS_O}: ${GUI_GLOBALS_CPP} ${GUI_GLOBALS_H} ${BOARDGUI_H}
	@echo "Creating ${GUI_GLOBALS_O}"
	${CXX} -c ${GUI_GLOBALS_CPP} ${flags} -o ${GUI_GLOBALS_O}

${ZENITY_O): ${ZENITY_CPP} ${ZENITY_H}
	@echo "Creating ${ZENITY_O}"
	@${CSS} -c ${ZENITY_CPP} ${flags} -o ${ZENITY_O}

clean:
	rm objectfiles/*.o

gui: ${GUI_CPP} ${BOARDGUI_O} ${GUI_GLOBALS_O} ${BOARDGUI_HOF_O}
	@echo "creating gui test script"
	${CXX} ${GUI_CPP} ${BOARDGUI_O} ${GUI_GLOBALS_O} ${BOARDGUI_HOF_O} ${flags} -o gui.out

