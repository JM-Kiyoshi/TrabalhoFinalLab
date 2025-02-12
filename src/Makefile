all:
	gcc *.c -o main -O3
	
clean:
	rm -r *.o main

sse:
	./main a.pgm resultado.pgm 1

kmeans:
	./main a.pgm resultado.pgm 2

separado:
	gcc -c kmeans.c -o kmeans.o
	gcc -c main.c -o main.o

compilar:
	gcc main.o kmeans.o -o main -O3