all:
	gcc *.c -o main -O3
	
clean:
	rm -r *.o main

sse:
	./main ./entrada ./saida 1 a.pgm

histograma:
	./main ./entrada ./saida 2 a.pgm

kmeans:
	./main ./entrada ./saida 3 a.pgm

separado:
	gcc -c kmeans.c -o kmeans.o
	gcc -c main.c -o main.o

compilar:
	gcc main.o kmeans.o -o main -O3
