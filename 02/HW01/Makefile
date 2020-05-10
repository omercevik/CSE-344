all:
	gcc -o programA programA.c
	./programA -i inputPathA.txt -o outputPathA.txt -t 50
	gcc -o programB programB.c -lm
	./programB -i outputPathA.txt -o outputPathB.txt -t 50
	gcc -o programC programC.c -lm
	./programC -i outputPathB.txt

v:
	gcc -o programA programA.c
	valgrind -v ./programA -i inputPathA.txt -o outputPathA.txt -t 50
	gcc -o programB programB.c -lm
	valgrind -v ./programB -i outputPathA.txt -o outputPathB.txt -t 50
	gcc -o programC programC.c -lm
	valgrind -v ./programC -i outputPathB.txt

a:
	gcc -o programA programA.c
	./programA -i inputPathA.txt -o outputPathA.txt -t 50

a1:
	gcc -o programA1 programA.c
	./programA1 -i inputPathA1.txt -o outputPathA.txt -t 50

a2:
	gcc -o programA2 programA.c
	./programA2 -i inputPathA2.txt -o outputPathA.txt -t 40

b:
	gcc -o programB programB.c -lm
	./programB -i outputPathA.txt -o outputPathB.txt -t 50

b1:
	gcc -o programB1 programB.c -lm
	./programB1 -i outputPathA.txt -o outputPathB.txt -t 50

b2:
	gcc -o programB2 programB.c -lm
	./programB2 -i outputPathA.txt -o outputPathB.txt -t 30

c:
	gcc -o programC programC.c -lm
	./programC -i outputPathB.txt

av:
	gcc -o programA programA.c
	valgrind -v ./programA -i inputPathA.txt -o outputPathA.txt -t 50

bv:
	gcc -o programB programB.c -lm
	valgrind -v ./programB -i outputPathA.txt -o outputPathB.txt -t 50

cv:
	gcc -o programC programC.c -lm
	valgrind -v ./programC -i outputPathB.txt

clean:
	rm -rf programA programA1 programA2 programB programB1 programB2 programC