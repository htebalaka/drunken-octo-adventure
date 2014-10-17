
GCC=g++
FLAGS=-lpanel -lncurses -std=c++11

BOARDGUI_CPP=src/BoardGUI.cpp
BOARDGUI_H=headers/BoardGUI.h
BOARDGUI_HOF_H=headers/BoardGUI_hof.h
BOARDGUI_O=objects/BoardGUI.o
MAIN_CPP=src/main.cpp
MAIN_O=objects/main.o

.PHONY: clean

clean:
	rm ${BOARDGUI_O} ${MAIN_O}

${BOARDGUI_O}: ${BOARDGUI_CPP} ${BOARDGUI_H} ${BOARDGUI_HOF_H}
	@echo "Creating ${BOARDGUI_O}"
	${GCC} -c ${BOARDGUI_CPP} ${FLAGS} -o ${BOARDGUI_O}

# not used
${MAIN_O}: ${BOARDGUI_H} ${MAIN_CPP}
	${GCC} -c ${MAIN_CPP} ${FLAGS} -o ${MAIN_O}

all: ${BOARDGUI_O} ${MAIN_CPP}
	@echo "Creating main.out"
	${GCC} ${BOARDGUI_O} ${MAIN_CPP} ${FLAGS} -o bin/main.out
