CC=gcc
LFLAGS=-lncurses
CFLAGS=

minesweeper: window.o libminesweeper.o array.o
	$(CC) -o bin/minesweeper main.c minesweeper.o array.o $< $(LFLAGS) $(CFLAGS)

window.o: window.c
	$(CC) -c $< $(CFLAGS)

libminesweeper.o: libminesweeper/minesweeper.c array.o
	$(CC) -c $<

array.o:
	$(CC) -c utils/array.c