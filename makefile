CC=gcc 
CFLAGS=-Wpedantic -std=gnu99 -Wall -g

all: dine hole

dine: dine.c dine.h
	$(CC) $(CFLAGS) dine.c -o dine -lpthread -lm

dine.o: dine.c dine.h
	$(CC) $(CFLAGS) -c dine.c -o dine.o -lpthread -lm

hole: hole.c hole.h list.o
	$(CC) $(CFLAGS) hole.c list.o -o hole -lpthread -lm

hole.o: hole.c hole.h list.o
	$(CC) $(CFLAGS) -c hole.c list.o -o hole.o -lpthread -lm

list.o: list.c
	$(CC) $(CFLAGS) -c list.c -o list.o -lpthread -lm

clean:
	rm -f *.o dine hole
