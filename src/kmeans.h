/*
    Alunos: Juliano Magalhães Jurity & João Victor de Lima
    Matrícula: 20241045050456
    Avaliação 04: Trabalho Final
    04.505.23 - 2024.2 - Prof Daniel Ferreira
    Compilador: gcc versão 13.2.0

*/

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
struct pgm clusterizacao(struct pgm *pio, char *filename, int *v, int k, int *vetorSoma, int *vetorContador); //aparentemente ok
double calcularInercia(struct pgm *pio, int *v, int k); 
int retornaQuantidadePixelIgual(struct pgm *img1, struct pgm *img2); // aparentemente funcionando bem
double dice(struct pgm *img1, struct pgm *img2);
int retornaTotalDePixel(struct pgm *img1, struct pgm *img2); //ok