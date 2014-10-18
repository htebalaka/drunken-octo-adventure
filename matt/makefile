all: objects/mysql.o
	@echo "DONE"

objects/mysql.o: headers/globalConstants.h sourcefiles/mysql.cpp
	g++ -c sourcefiles/mysql.cpp -o objects/mysql.o
