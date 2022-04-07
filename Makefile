CC=g++
CFLAGS=-c -Wall -std=c++2a

ifeq ($(OS), Winsows_NT)
	RM = del
	OUTNAME = main.exe
else
	RM = rm -rf
	OUTNAME = main.out
endif

all:
	$(CC) main.cpp -o $(OUTNAME)

clean:
	$(RM) *.out *.o *.h.gch *.txt

run: all
	./$(OUTNAME)