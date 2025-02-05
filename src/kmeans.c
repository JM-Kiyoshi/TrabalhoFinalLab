/*
    Alunos: Juliano Magalhães Jurity & João Victor de Lima
    Matrícula: 20241045050456 & 20241045050057
    Avaliação 04: Trabalho Final
    04.505.23 - 2024.2 - Prof Daniel Ferreira
    Compilador: gcc versão 13.2.0

*/

#include <stdio.h>
#include <stdlib.h>
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
        int index = rand() % (pio->r * pio->c);
        v[i] = pio->pData[index];
    }
}

double calcularInercia(struct pgm *pio, int *v, int k){
    double inercia = 0.0;
    
    for (int i = 0; i < pio->r; i++) {
        for (int j = 0; j < pio->c; j++) {
            int index = i * pio->c + j;
            int menorDist = (pio->pData[index] - v[0]);

            for (int m = 1; m < k; m++) {
                int dist = (pio->pData[index] - v[m]);
                if (dist < menorDist) {
                    menorDist = dist;
                }
            }

            inercia += menorDist;
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
                int index = i * pio->c + j;
                int clusterIndex = retornaMenorDistancia(v, k, pio->pData[index], vetorSoma, vetorContador);
                novaImagem.pData[index] = v[clusterIndex];
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



int retornaQuantidadePixelIgual(struct pgm *img1, struct pgm *img2){
	int contador = 0;
	if((img1->r*img1->c) == (img2->r*img2->c)){
		for (int i = 0; i < img1->r; i++)
		{
			for (int j = 0; j < img1->c; j++)
			{
				int index = i * img1->c + j;
				if(img1->pData[index] == img2->pData[index]){
					contador++;
				}
			}
			
		}
	}
	else{
		return 0;
	}
	
	return contador;
}

int retornaTotalDePixel(struct pgm *img1, struct pgm *img2){
	int contador1 = 0;
	int contador2 = 0;
	for (int i = 0; i < img1->r * img1->c; i++)
	{
		contador1++;
	}

	for (int i = 0; i < img2->r * img2->c; i++)
	{
		contador2++;
	}
	
	return contador1+contador2;
}

double dice(struct pgm *img1, struct pgm *img2){
	double resultado = 0;
	int numPixelIgual = retornaQuantidadePixelIgual(img1, img2);
	int numPixelTotal = retornaTotalDePixel(img1, img2);
	printf("numero de pixel igual: %d\n", numPixelIgual);
	printf("numero de pixel total: %d\n", numPixelTotal);
	resultado = 2*((double)numPixelIgual)/numPixelTotal;

	return resultado;
}