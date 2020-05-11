CC = gcc
CFLAGS = -c
VALG = valgrind -v
SHD = -lrt -lpthread

all:main run

main: program.o supplier.o cook.o student.o shdmem.o fileOperations.o
	$(CC) program.o supplier.o cook.o student.o shdmem.o fileOperations.o -o program $(SHD)

program.o: program.c
	$(CC) $(CFLAGS) program.c

supplier.o: supplier.c
	$(CC) $(CFLAGS) supplier.c

cook.o: cook.c
	$(CC) $(CFLAGS) cook.c

student.o: student.c
	$(CC) $(CFLAGS) student.c

shdmem.o: shdmem.c
	$(CC) $(CFLAGS) shdmem.c

fileOperations.o: fileOperations.c
	$(CC) $(CFLAGS) fileOperations.c

run:
	./program -N 3 -M 12 -T 5 -S 4 -L 13 -F filePath.txt

v:main runv

runv:
	$(VALG) ./program -N 3 -M 12 -T 5 -S 4 -L 13 -F filePath.txt

clean:
	rm -rf *o program