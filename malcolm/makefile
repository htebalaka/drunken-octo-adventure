
GCC=g++
FLAGS=-lpanel -lncurses -std=c++11

GUI_GLOBALS_H=headers/GUI_Globals.h
GUI_GLOBALS_CPP=src/GUI_Globals.cpp
GUI_GLOBALS_O=objects/GUI_Globals.o

BOARDGUI_HOF_H=headers/BoardGUI_hof.h

BOARDGUI_H=headers/BoardGUI.h
BOARDGUI_CPP=src/BoardGUI.cpp
BOARDGUI_O=objects/BoardGUI.o

MAIN_CPP=src/main.cpp
MAIN_O=objects/main.o

.PHONY: clean

clean:
	@echo "Cleaning object files"
	rm ${BOARDGUI_O} ${MAIN_O} ${BOARD_GUI_O}

${BOARDGUI_O}: ${BOARDGUI_CPP} ${BOARDGUI_H} ${BOARDGUI_HOF_H}
	@echo "Creating ${BOARDGUI_O}"
	${GCC} -c ${BOARDGUI_CPP} ${FLAGS} -o ${BOARDGUI_O}
${GUI_GLOBALS_O}: ${GUI_GLOBALS_CPP} ${GUI_GLOBALS_H}
	@echo "Creating ${GUI_GLOBALS_O}"
	${GCC} -c ${GUI_GLOBALS_CPP} ${FLAGS} -o ${GUI_GLOBALS_O}

all: ${BOARDGUI_O} ${MAIN_CPP} ${GUI_GLOBALS_O}
	@echo "Creating main.out"
	${GCC} ${BOARDGUI_O} ${GUI_GLOBALS_O} ${MAIN_CPP} ${FLAGS} -o bin/main.out
