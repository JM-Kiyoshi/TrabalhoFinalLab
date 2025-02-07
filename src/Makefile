all:
	gcc *.c -o main -O3
	
clean:
	rm -r *.o main

sse:
	./main a.pgm resultado.pgm 1

kmeans:
	./main a.pgm resultado.pgm 2

separado:
	gcc -c kmeans.c
	gcc -c main.c

compilar:
	gcc -o main kmeans.o main.c -O3