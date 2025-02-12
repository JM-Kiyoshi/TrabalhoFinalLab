# K-Means

O agrupamento k-means é um método de segregar em torno de diversos centroids, criando clusterings gerando o efeito de particionar as observações (no caso do trabalho final, pixels de uma imagem) em K grupos onde cada observação pertence a um grupo mais próximo da média. 

## Clonagem

Primeiramente deve ser feita a clonagem do repositório em sua máquina, podendo ser pelo método HTTPS ou SSH

```bash
git clone https://github.com/JM-Kiyoshi/TrabalhoFinalLab.git
```
ou
```bash
git clone git@github.com:JM-Kiyoshi/TrabalhoFinalLab.git
````

## Forma de compilar e rodar o programa

É possível compilar o programa de forma separada ou através do makefile presente no repositório, seguem os comandos

### Compilação separada
criando o arquivo .o através de cada arquivo.c
```bash
gcc -c kmeans.c -o kmeans.o
gcc -c main.c -o main.o
````
Compilando os arquivos .o em um executável
```bash
gcc main.o kmeans.o -o main -O3
````
Executando arquivo no terminal
```bash
./main Foto_a_ser_lida.pgm Resultado.pgm 1
./main Foto_a_ser_lida.pgm Resultado.pgm 2
````
1 - Para testar qual o K ideal para a imagem que foi lida, roda de k = 1 até k = 10.

2 - Para utilizar o k-means de fato e gerar um txt com os dados para montar o histograma.

### MakeFile

Compilando através do makefile

Através deste comando é possível gerar os arquivos .o que foram indicados no makefile
```bash
make separado
````

Através deste comando é possível gerar o executável baseado nos arquivos .o
```bash
make compilar
````

Através destes comandos é possível utilizar a função que testa os K para a imagem e gerar a imagem clusterizada respectivamente.
```bash
make sse
ou
make kmeans
````

## Gerando os gráficos do cotovelo e o histograma
Foi utilizada a biblioteca matplotlib do python para gerar os gráficos de forma mais fácil.

### Código para gerar o gráfico do SSE
Os dados deste código foram recebidos da função presente no código.
```python
import matplotlib.pyplot as plt

# Dados fornecidos
k_values = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
inercia_values = [4852936.00,812805.00,340234.00,213425.00,280407.00,125721.00,105325.00,70091.00,70658.00,63757.00]

# Criando o gráfico
plt.figure(figsize=(8, 5))
plt.plot(k_values, inercia_values, marker='o', linestyle='-', color='b')

# Adicionando rótulos e título
plt.xlabel("Número de Clusters (k)")
plt.ylabel("Inércia")
plt.title("Método do Cotovelo - Inércia vs Número de Clusters")
plt.grid(True)

# Exibindo o gráfico
plt.show()
```

### Código para gerar o histograma da imagem original
```python
import matplotlib.pyplot as plt

# Dados fornecidos
dados = [1,1,3,1,5,8,27,8,21,61,89,103,134,141,67,77,149,151,156,132,128,45,38,62,85,83,109,54,44,31,42,64,66,70,71,60,40,58,84,110,112,116,99,47,54,78,36,34,40,25,18,20,25,13,26,10,7,6,10,18,15,7,8,4,9,13,12,10,15,11,3,3,9,12,12,10,18,17,9,9,23,22,45,39,45,37,26,38,58,48,43,50,36,20,45,57,36,35,30,26,19,25,51,35,49,31,21,11,21,26,10,19,19,6,9,16,7,5,6,4,1,1]

# Criando o histograma
plt.figure(figsize=(10, 6))
plt.hist(dados, bins=range(min(dados), max(dados) + 2), edgecolor='black', alpha=0.7)
plt.xlabel('Valores')
plt.ylabel('Frequência')
plt.title('Histograma dos Dados')
plt.grid(axis='y', linestyle='--', alpha=0.7)

# Exibindo o gráfico
plt.show()
```
