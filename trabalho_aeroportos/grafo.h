#ifndef GRAFO_H
#define GRAFO_H

#include <stdbool.h>

#define MAX_AEROPORTOS 50
#define CODIGO_SIZE 4 
#define CIDADE_SIZE 50
//struct criada para representar um aeroporto, contendo o código do aeroporto e a cidade onde ele está localizado
typedef struct {
    char codigo[CODIGO_SIZE];
    char cidade[CIDADE_SIZE];
} Aeroporto;
/*struct criada para representar o grafo de aeroportos, contendo um array de aeroportos, uma matriz de adjacência 
para representar os voos entre os aeroportos, o número de aeroportos cadastrados e a capacidade máxima do grafo*/
typedef struct {
    Aeroporto *aeroportos;
    int **matriz_adjacencia;
    int num_aeroportos;
    int capacidade;
} GrafoAeroportos;

//protótipos das funções para manipular o grafo de aeroportos
GrafoAeroportos* criar_grafo();
void destruir_grafo(GrafoAeroportos *grafo);
bool adicionar_aeroporto(GrafoAeroportos *grafo, const char *codigo, const char *cidade);
bool adicionar_voo(GrafoAeroportos *grafo, const char *origem, const char *destino, int numero_voo);
bool remover_voo(GrafoAeroportos *grafo, const char *origem, const char *destino, int numero_voo);
void listar_voos_saida(GrafoAeroportos *grafo, const char *codigo_aeroporto);
void listar_trajetos(GrafoAeroportos *grafo, const char *origem, const char *destino);
int encontrar_indice_aeroporto(GrafoAeroportos *grafo, const char *codigo);

#endif