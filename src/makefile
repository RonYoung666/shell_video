all:
	gcc -c -g 24BitImage.c
	gcc -c -g test.c
	gcc 24BitImage.o test.o -o test
	rm -f *.o

clean:
	rm -f *.o
	rm -f test
