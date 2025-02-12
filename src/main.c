/*
    Alunos: Juliano Magalhães Jurity & João Victor de Lima
    Matrícula: 20241045050456 & 20241045050057
    Avaliação 04: Trabalho Final
    04.505.23 - 2024.2 - Prof Daniel Ferreira
    Compilador: gcc versão 13.2.0

*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <dirent.h>
#include <string.h>
#include "kmeans.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    if (argc != 5) {
        printf("Formato: \n\t %s <diretorioEntrada> <diretorioSaida> <K clusters> <teste>\n", argv[0]);
        exit(1);
    }

    clock_t begin = clock();

    struct pgm img;
    
    int op = atoi(argv[3]);

    if(op == 1){
        FILE *fp = fopen("inercia.txt", "w");
    
        if (fp == NULL) {
        printf("Erro ao abrir o arquivo\n");
            exit(1);
        }
        
        printf("Vamos calcular de k = 1 ate k = 10\n");
        
        fprintf(fp, "[");
        for (int k = 1; k <= 10; k++) {
            printf("Executando K-Means para K = %d...\n", k);
            readPGMImage(&img, argv[4]);
            int *vetor = (int *)malloc(k * sizeof(int));
            int *vetorContadores = (int *)calloc(k, sizeof(int));
            int *vetorSomatorio = (int *)calloc(k, sizeof(int));

            for (int i = 0; i < k; i++) {
                int index = rand() % (img.r * img.c);
                vetor[i] = img.pData[index];
            }

            struct pgm resultado = clusterizacao(&img, argv[4], vetor, k, vetorSomatorio, vetorContadores);
            double inercia = calcularInercia(&img, vetor, k);
            writePGMImage(&resultado, "teste.pgm");
            fprintf(fp, ",%.2f", inercia);
            printf("K=%d, Inércia=%.2f\n", k, inercia);

            free(vetor);
            free(vetorContadores);
            free(vetorSomatorio);
        }
        fprintf(fp, "]");
        fclose(fp);
    }

    else if(op == 2){
        int k;
        
        printf("Digite o numero de K: ");
        scanf("%d", &k);
        
        int *vetor = (int *)malloc(k * sizeof(int));
        int *vetorContadores = (int *)calloc(k, sizeof(int));
        int *vetorSomatorio = (int *)calloc(k, sizeof(int));

        readPGMImage(&img, argv[4]);
        gerarCentroids(&img, vetor, k);

        struct pgm resultado = clusterizacao(&img, argv[4], vetor, k, vetorSomatorio, vetorContadores);
        criarHistograma(&img);


        free(vetor);
        free(vetorContadores);
        free(vetorSomatorio);
    }

    else if(op == 3){
        DIR *d;
        struct dirent *dir;
        d = opendir("entrada");
	
	    char filepath[1024]; // buffer para armazenar o caminho completo
        char outpath[1024]; // mesma coisa para o caminho de saida

        int k;
        printf("Digite o numero de K: ");
        scanf("%d", &k);

        int *vetor = (int *)malloc(k * sizeof(int));
        int *vetorContadores = (int *)calloc(k, sizeof(int));
        int *vetorSomatorio = (int *)calloc(k, sizeof(int));



        // laço de repetição para percorrer todo o diretório de entrada
        while ((dir = readdir(d)) != NULL){
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) { //pra se livrar de um bug
                continue;
            }

        snprintf(filepath, sizeof(filepath), "%s/%s", argv[1], dir->d_name); //formatando o caminho para "filepath"
        printf("%s\n", filepath);
    
        readPGMImage(&img, filepath);
        gerarCentroids(&img, vetor, k);

		struct pgm resultado = clusterizacao(&img, dir->d_name, vetor, k, vetorSomatorio, vetorContadores);
        
        // Gravando a imagem processada
        snprintf(outpath, sizeof(outpath), "%s/out-%s",  argv[2],dir->d_name); // formatando saida

        writePGMImage(&resultado, outpath);

    }

    closedir(d);
    }

    double end = (double)(clock() - begin) / CLOCKS_PER_SEC;

    printf("%lf segundos\n", end);

    return 0;
}
