CXX=g++
CFLAGS=-c -Wall --std=c++11
DEBUG=

ifeq ($(OS),Windows_NT)
    RM := del
	BINARY := main.exe
else
    RM := rm
	BINARY := main
endif

all: main

debug: DEBUG = -g
debug: main

main: main.o 
	$(CXX) $(DEBUG) main.o  -o main

main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp

clean:
	$(RM) *.o *.gch $(BINARY)

run: all
	$(BINARY) 0 0 1 1
