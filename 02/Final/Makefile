CC = gcc
CFLAGS = -c -Wall
VALG = valgrind -v --leak-check=full
SHD = -lrt -lpthread

all:server client

server: server.o graph.o
	$(CC) server.o graph.o -o server $(SHD)

client: client.o graph.o
	$(CC) client.o graph.o -o client $(SHD)

server.o: server.c
	$(CC) $(CFLAGS) server.c

client.o: client.c
	$(CC) $(CFLAGS) client.c

graph.o: graph.c
	$(CC) $(CFLAGS) graph.c

rs:
	./server -i input.txt -p 34567 -o log.txt -s 4 -x 24

rc:
	./client -a 127.0.0.1 -p 34567 -s 0 -d 6300

vs:all rsv

vc:all rcv

rsv:
	$(VALG) ./server -i input.txt -p 34567 -o log.txt -s 4 -x 24

rcv:
	$(VALG) ./client -a 127.0.0.1 -p 34567 -s 0 -d 1

clean:
	rm -rf *o server client