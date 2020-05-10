all:
	gcc -o program program.c -lm
	./program -i inputPath.txt -o outputPath.txt

v:
	gcc -o program program.c -lm
	valgrind -v ./program -i inputPath.txt -o outputPath.txt

clean:
	rm -rf program outputPath.txt