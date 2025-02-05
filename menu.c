#include <stdio.h>
#include <stdlib.h>
#include "kmeans.h"



void opcoes(){
    printf("1 - Gerar inercia do arquivo informado.\n");
    printf("2 - Gerar imagem clusterizada do arquivo informado.\n");
}

void menu(struct pgm *pio, char *filename, int *v, int k, int *vetorSoma, int *vetorContador){
    int op;
    int flag = 1;
    opcoes();
    printf("===================================\n");
    printf("Digite uma das opcoes: ");
    if(scanf("%d", &op) == 0){
        printf("Entrada invalida.\n");
        exit(1);
    }
    while(flag != 0){
        switch (op)
        {
        case 1:
            printf("Vamos calcular de k = 1 ate k = 10\n");
            for (int i = 1; i < 10; i++)
            {
                double inercia = calcularInercia(pio, v, k);
                fprintf(fp, "%d %.2f\n", k, inercia); //preciso escrever no txt para resolver esse bug
                printf("K=%d, Inércia=%.2f\n", k, inercia);
        
            }
            menu(pio, filename, v, k, vetorSoma, vetorContador);
            break;

        case 2:
            clusterizacao(pio, filename, v, k, vetorSoma, vetorContador);
            menu(pio, filename, v, k, vetorSoma, vetorContador);
            break;
        
        default:
            menu(pio, filename, v, k, vetorSoma, vetorContador);
            break;
        case 0:
            flag = 0;
            break;
        }
    }
}