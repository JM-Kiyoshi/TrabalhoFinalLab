/*
    Alunos: Juliano Magalhães Jurity & João Victor de Lima
    Matrícula: 20241045050456 & 20241045050057
    Avaliação 04: Trabalho Final
    04.505.23 - 2024.2 - Prof Daniel Ferreira
    Compilador: gcc versão 13.2.0

*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "kmeans.h"

#define MAX_ITERACOES 50


void readPGMImage(struct pgm *pio, char *filename){


	FILE *fp;
	char ch;


	if (!(fp = fopen(filename,"r"))){
		perror("Erro.");
		exit(1);
	}


	if ( (ch = getc(fp))!='P'){
		puts("A imagem fornecida não está no formato pgm");
		exit(2);
	}
	
	pio->tipo = getc(fp)-48;
	
	fseek(fp,1, SEEK_CUR);


	while((ch=getc(fp))=='#'){
		while( (ch=getc(fp))!='\n');
	}


	fseek(fp,-1, SEEK_CUR);


	fscanf(fp, "%d %d",&pio->c,&pio->r);
	if (ferror(fp)){ 
		perror(NULL);
		exit(3);
	}	
	fscanf(fp, "%d",&pio->mv);
	fseek(fp,1, SEEK_CUR);


	pio->pData = (unsigned char*) malloc(pio->r * pio->c * sizeof(unsigned char));


	switch(pio->tipo){
		case 2:
			puts("Lendo imagem PGM (dados em texto)");
			for (int k=0; k < (pio->r * pio->c); k++){
				fscanf(fp, "%hhu", pio->pData+k);
			}
		break;	
		case 5:
			puts("Lendo imagem PGM (dados em binário)");
			fread(pio->pData,sizeof(unsigned char),pio->r * pio->c, fp);
		break;
		default:
			puts("Não está implementado");
	}
	
	fclose(fp);


}


void writePGMImage(struct pgm *pio, char *filename){
	FILE *fp;
	char ch;


	if (!(fp = fopen(filename,"wb"))){
		perror("Erro.");
		exit(1);
	}


	fprintf(fp, "%s\n","P5");
	fprintf(fp, "%d %d\n",pio->c, pio->r);
	fprintf(fp, "%d\n", 255);


	fwrite(pio->pData, sizeof(unsigned char),pio->c * pio->r, fp);


	fclose(fp);


}

void gerarCentroids(struct pgm *pio, int *v, int k){
    for (int i = 0; i < k; i++){
        int indice = rand() % (pio->r * pio->c);
        v[i] = pio->pData[indice];
    }
}

double calcularInercia(struct pgm *pio, int *v, int k) {
    double inercia = 0.0;
    
    for (int i = 0; i < pio->r; i++) {
        for (int j = 0; j < pio->c; j++) {
            int indice = i * pio->c + j;
            int valorPixel = pio->pData[indice];
            
            
            int clusterMaisProximo = 0;
            int menorDistanciaQuadrada = (valorPixel - v[0]) * (valorPixel - v[0]);
            
            for (int m = 1; m < k; m++) {
                int distanciaQuadrada = (valorPixel - v[m]) * (valorPixel - v[m]);
                if (distanciaQuadrada < menorDistanciaQuadrada) {
                    menorDistanciaQuadrada = distanciaQuadrada;
                    clusterMaisProximo = m;
                }
            }
            
            
            inercia += menorDistanciaQuadrada;
        }
    }
    
    return inercia;
}

int retornaMenorDistancia(int *v, int k, int valor, int *vetorSoma, int *vetorContador){
	int indiceAux = 0;
    int menorDistancia = 255;

    for (int i = 0; i < k; i++) {  
        int distancia = abs((int)valor - (int)v[i]);;
        if (distancia < menorDistancia) {
            menorDistancia = distancia;
            indiceAux = i;
        }
    }

    vetorSoma[indiceAux] += valor;
    vetorContador[indiceAux]++;

    return indiceAux;
}


void zeraVetorSomaEContador(int k, int *vetorSoma, int *vetorContador){
	for (int i = 0; i < k; i++)
	{
		*(vetorSoma+i) = 0;
		*(vetorContador+i) = 0;
	}
}

int gerarNovosClusters(struct pgm *pio, int *v, int k, int *vetorSoma, int *vetorContador){
	int flag = 0;
	for (int i = 0; i < k; i++)
	{
		if(vetorContador[i] != 0){
			int novoCluster = vetorSoma[i] / vetorContador[i];
			
			if(abs(v[i] - novoCluster) > 1){
				v[i] = novoCluster;
				flag = 1;
			}
		}
		else{
			v[i] = rand() % (pio->mv + 1);
			flag = 1;
		}
	}
	return flag;
}

struct pgm clusterizacao(struct pgm *pio, char *filename, int *v, int k, int *vetorSoma, int *vetorContador){
	for (int i = 0; i < k; i++) {
		v[i] = rand()%(pio->mv+1);
    }
	int iteracoes = 0;
    int mudou;
	struct pgm novaImagem;
	novaImagem.r = pio->r;
	novaImagem.c = pio->c;
	novaImagem.mv = pio->mv;

	novaImagem.pData = (unsigned char *) malloc(novaImagem.r * novaImagem.c * sizeof(unsigned char));
	

    do {
        zeraVetorSomaEContador(k, vetorSoma, vetorContador);
        for (int i = 0; i < pio->r; i++) {
            for (int j = 0; j < pio->c; j++) {
                int indice = i * pio->c + j;
                int clusterIndice = retornaMenorDistancia(v, k, pio->pData[indice], vetorSoma, vetorContador);
                novaImagem.pData[indice] = v[clusterIndice];
            }
        }

        mudou = gerarNovosClusters(pio, v, k, vetorSoma, vetorContador);
		iteracoes++;

		if(iteracoes > MAX_ITERACOES){
			break;
		}
    } while (mudou != 0);

	return novaImagem;
}

void criarHistograma(struct pgm *pio){
	int *histograma = (int *) calloc(256, sizeof(int));
	for (int i = 0; i < pio->r * pio->c; i++)
	{
		int indice = pio->pData[i];
		histograma[indice] += 1;
	}

	FILE *fp = fopen("histograma.txt", "w");
		fprintf(fp, "dados = [");
        for (int i = 1; i < 256; i++)
        {
            if(histograma[i] != 0){
				fprintf(fp, ",%d", histograma[i]);
			}
        }
		fprintf(fp, "]");
        
        if (fp == NULL) {
        printf("Erro ao abrir o arquivo\n");
            exit(1);
        }
	free(histograma);
}

void LerDiretorio(struct pgm *pio, int *v, int k, int *vetorSoma, int *vetorContador){
}