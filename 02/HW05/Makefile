CC = gcc
CFLAGS = -c -Wall
GFLAG = -g
VALG = valgrind -v
THREAD = -lpthread

all:main run

main: main.o florist.o client.o
	$(CC) $(GFLAG) main.o florist.o client.o -o floristApp $(THREAD)

main.o: main.c
	$(CC) $(GFLAG) $(CFLAGS) main.c

florist.o: florist.c
	$(CC) $(GFLAG) $(CFLAGS) florist.c

client.o: client.c
	$(CC) $(GFLAG) $(CFLAGS) client.c

run:
	./floristApp -i data.dat

v:main runv

runv:
	$(VALG) ./floristApp -i data.dat

clean:
	rm -rf *o floristApp