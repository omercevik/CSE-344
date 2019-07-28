CC = gcc
CFLAGS = -pedantic -errors -Wall -c -std=gnu99 

all: buNeDuFork

buNeDuFork: hw2.o
	$(CC) hw2.o -o buNeDuFork

hw2.o: hw2.c
	$(CC) $(CFLAGS) hw2.c

clean:
	rm -rf *o buNeDuFork