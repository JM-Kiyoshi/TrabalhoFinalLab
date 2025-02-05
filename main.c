/*
    Alunos: Juliano Magalhães Jurity & João Victor de Lima
    Matrícula: 20241045050456
    Avaliação 04: Trabalho Final
    04.505.23 - 2024.2 - Prof Daniel Ferreira
    Compilador: gcc versão 13.2.0

*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "kmeans.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc != 4) {
        printf("Formato: \n\t %s <imagemEntrada.pgm> <imagemSaida.pgm> <o que deseja fazer: 1 - calcular inercia, 2 - clusterizaçao>\n", argv[0]);
        exit(1);
    }

    struct pgm img;
    struct pgm teste;
    readPGMImage(&img, argv[1]);
    readPGMImage(&teste, argv[1]);

    int op = atoi(argv[3]);

    if(op == 1){
        FILE *fp = fopen("inercia.txt", "w");
    
        if (fp == NULL) {
        printf("Erro ao abrir o arquivo\n");
            exit(1);
        }
        
        printf("Vamos calcular de k = 1 ate k = 10\n");
        
        for (int k = 1; k <= 10; k++) {
            printf("Executando K-Means para K = %d...\n", k);
            readPGMImage(&img, argv[2]);
            int *vetor = (int *)malloc(k * sizeof(int));
            int *vetorContadores = (int *)calloc(k, sizeof(int));
            int *vetorSomatorio = (int *)calloc(k, sizeof(int));

            for (int i = 0; i < k; i++) {
                int index = rand() % (img.r * img.c);
                vetor[i] = img.pData[index];
            }

            clusterizacao(&img, argv[2], vetor, k, vetorSomatorio, vetorContadores);
            double inercia = calcularInercia(&img, vetor, k);
                    
            fprintf(fp, "%d %.2f\n", k, inercia);
            printf("K=%d, Inércia=%.2f\n", k, inercia);

            free(vetor);
            free(vetorContadores);
            free(vetorSomatorio);
        }
        fclose(fp);
    }

    else if(op == 2){
        int k;
        printf("Digite o numero de K: ");
        scanf("%d", &k);
        int *vetor = (int *)malloc(k * sizeof(int));
        int *vetorContadores = (int *)calloc(k, sizeof(int));
        int *vetorSomatorio = (int *)calloc(k, sizeof(int));

        gerarCentroids(&img, vetor, k);

        struct pgm resultado = clusterizacao(&img, argv[2], vetor, k, vetorSomatorio, vetorContadores);
        writePGMImage(&resultado, argv[2]);
        double diceResultado = dice(&img, &resultado);
        printf("dice: %.2f\n", diceResultado);

        free(vetor);
        free(vetorContadores);
        free(vetorSomatorio);
    }


    return 0;
}
