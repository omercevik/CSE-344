CC = gcc
CFLAGS = -pedantic -errors -Wall -c -std=gnu99 

all: buNeDu

buNeDu: hw1.o
	$(CC) hw1.o -o buNeDu

hw1.o: hw1.c
	$(CC) $(CFLAGS) hw1.c

clean:
	rm -rf *o buNeDu