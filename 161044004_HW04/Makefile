CC = gcc
CFLAGS = -pedantic -errors -Wall -c -std=gnu99 

all: buNeDuFPF

buNeDuFPF: hw4.o
	$(CC) hw4.o -o buNeDuFPF

hw4.o: hw4.c
	$(CC) $(CFLAGS) hw4.c

clean:
	rm -rf *o buNeDuFPF