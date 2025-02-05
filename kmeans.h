#include <stdio.h>
#include <stdlib.h>


struct pgm{
	int tipo;
	int c;
	int r;
	int mv;
	unsigned char *pData;
};

void readPGMImage(struct pgm *, char *);
void viewPGMImage(struct pgm *);
void writePGMImage(struct pgm *, char *);
void gerarCentroids(struct pgm *pio, int *v, int k);
int retornaMenorDistancia(int *v, int k, int valor, int *vetorSoma, int *vetorContador);
void clusterizacao(struct pgm *pio, char *filename, int *v, int k, int *vetorSoma, int *vetorContador);
double calcularInercia(struct pgm *pio, int *v, int k);