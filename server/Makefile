CC = g++
CCFLAG =  -std=c++11
build: clean main.o
	$(CC) $(CCFLAG) main.o -o main

clean:
	-rm -rf *.o

main.o: main.cpp
	$(CC) $(CCFLAG) -c main.cpp
