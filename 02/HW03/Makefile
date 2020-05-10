CC = gcc
CFLAGS = -c
LIBC = -Wall -I/usr/local/include
LINKC = -L/usr/local/lib
MATH = -lgsl -lgslcblas -lm
GFLAG = -g
VALG = valgrind -v

all:main run

main: program.o
	$(CC) $(GFLAG) $(LINKC) program.o $(MATH) -o program

program.o: program.c
	$(CC) $(GFLAG) $(LIBC) $(CFLAGS) program.c

run:
	./program -i inputPathA.txt -j inputPathB.txt -n 2

v:main runv

runv:
	$(VALG) ./program -i inputPathA.txt -j inputPathB.txt -n 2

clean:
	rm -rf *o program