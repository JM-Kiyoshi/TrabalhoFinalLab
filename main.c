#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "kmeans.h"


int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc != 3) {
        printf("Formato: \n\t %s <imagemEntrada.pgm> <imagemSaida.pgm>\n", argv[0]);
        exit(1);
    }

    struct pgm img;
    readPGMImage(&img, argv[1]); 

	int k = 4;

    int *vetor = (int *)malloc(k * sizeof(int));
    int *vetorContadores = (int *)calloc(k, sizeof(int));
    int *vetorSomatorio = (int *)calloc(k, sizeof(int));

	gerarCentroids(&img, vetor, k);


	menu(&img, argv[2], vetor, k, vetorSomatorio, vetorContadores);

    free(vetor);
    free(vetorContadores);
    free(vetorSomatorio);

    return 0;
}
