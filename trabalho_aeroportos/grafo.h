#ifndef GRAFO_H
#define GRAFO_H

#include <stdbool.h>

#define MAX_AEROPORTOS 50
#define CODIGO_SIZE 4
#define CIDADE_SIZE 50

typedef struct {
    char codigo[CODIGO_SIZE];
    char cidade[CIDADE_SIZE];
} Aeroporto;
/* Estrutura do grafo de aeroportos, que tem um vetor de aeroportos e uma matriz de adjacencia
 para representar os voos entre os aeroportos*/
typedef struct {
    Aeroporto *aeroportos;
    int **matriz_adjacencia;
    int num_aeroportos;
    int capacidade;
} GrafoAeroportos;

// Protótipos das funções
GrafoAeroportos* criar_grafo();
void destruir_grafo(GrafoAeroportos *grafo);
bool adicionar_aeroporto(GrafoAeroportos *grafo, const char *codigo, const char *cidade);
bool adicionar_voo(GrafoAeroportos *grafo, const char *origem, const char *destino, int numero_voo);
bool remover_voo(GrafoAeroportos *grafo, const char *origem, const char *destino, int numero_voo);
void listar_voos_saida(GrafoAeroportos *grafo, const char *codigo_aeroporto);
void listar_trajetos(GrafoAeroportos *grafo, const char *origem, const char *destino);
int encontrar_indice_aeroporto(GrafoAeroportos *grafo, const char *codigo);

#endif