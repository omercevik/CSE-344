CC = gcc
CFLAGS = -c
VALG = valgrind -v
THREAD = -lpthread

all:main run

main: program.o wholesaler.o chefs.o semaphore.o
	$(CC) program.o wholesaler.o chefs.o semaphore.o -o program $(THREAD)

program.o: program.c
	$(CC) $(CFLAGS) program.c

wholesaler.o: wholesaler.c
	$(CC) $(CFLAGS) wholesaler.c

chefs.o: chefs.c
	$(CC) $(CFLAGS) chefs.c

semaphore.o: semaphore.c
	$(CC) $(CFLAGS) semaphore.c

run:
	./program -i filePath.txt

v:main runv

runv:
	$(VALG) ./program -i filePath.txt

clean:
	rm -rf *o program
