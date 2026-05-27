#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GrafoAeroportos* criar_grafo() {
    GrafoAeroportos *grafo = (GrafoAeroportos*)malloc(sizeof(GrafoAeroportos));
    if (!grafo) return NULL;
    
    grafo->capacidade = MAX_AEROPORTOS;
    grafo->num_aeroportos = 0;
    
    grafo->aeroportos = (Aeroporto*)malloc(grafo->capacidade * sizeof(Aeroporto));
    if (!grafo->aeroportos) {
        free(grafo);
        return NULL;
    }
    
    grafo->matriz_adjacencia = (int**)malloc(grafo->capacidade * sizeof(int*));
    if (!grafo->matriz_adjacencia) {
        free(grafo->aeroportos);
        free(grafo);
        return NULL;
    }
    
    for (int i = 0; i < grafo->capacidade; i++) {
        grafo->matriz_adjacencia[i] = (int*)calloc(grafo->capacidade, sizeof(int));
        if (!grafo->matriz_adjacencia[i]) {
            for (int j = 0; j < i; j++) {
                free(grafo->matriz_adjacencia[j]);
            }
            free(grafo->matriz_adjacencia);
            free(grafo->aeroportos);
            free(grafo);
            return NULL;
        }
    }
    
    return grafo;
}

void destruir_grafo(GrafoAeroportos *grafo) {
    if (!grafo) return;
    
    for (int i = 0; i < grafo->capacidade; i++) {
        free(grafo->matriz_adjacencia[i]);
    }
    free(grafo->matriz_adjacencia);
    free(grafo->aeroportos);
    free(grafo);
}

int encontrar_indice_aeroporto(GrafoAeroportos *grafo, const char *codigo) {
    for (int i = 0; i < grafo->num_aeroportos; i++) {
        if (strcmp(grafo->aeroportos[i].codigo, codigo) == 0) {
            return i;
        }
    }
    return -1;
}

bool adicionar_aeroporto(GrafoAeroportos *grafo, const char *codigo, const char *cidade) {
    if (grafo->num_aeroportos >= grafo->capacidade) {
        printf("Capacidade máxima de aeroportos atingida.\n");
        return false;
    }
    
    if (encontrar_indice_aeroporto(grafo, codigo) != -1) {
        printf("Aeroporto com código %s já existe.\n", codigo);
        return false;
    }
    
    strncpy(grafo->aeroportos[grafo->num_aeroportos].codigo, codigo, CODIGO_SIZE - 1);
    grafo->aeroportos[grafo->num_aeroportos].codigo[CODIGO_SIZE - 1] = '\0';
    
    strncpy(grafo->aeroportos[grafo->num_aeroportos].cidade, cidade, CIDADE_SIZE - 1);
    grafo->aeroportos[grafo->num_aeroportos].cidade[CIDADE_SIZE - 1] = '\0';
    
    grafo->num_aeroportos++;
    printf("Aeroporto %s - %s cadastrado com sucesso.\n", codigo, cidade);
    return true;
}

bool adicionar_voo(GrafoAeroportos *grafo, const char *origem, const char *destino, int numero_voo) {
    int indice_origem = encontrar_indice_aeroporto(grafo, origem);
    int indice_destino = encontrar_indice_aeroporto(grafo, destino);
    
    if (indice_origem == -1) {
        printf("Aeroporto de origem %s não encontrado.\n", origem);
        return false;
    }
    
    if (indice_destino == -1) {
        printf("Aeroporto de destino %s não encontrado.\n", destino);
        return false;
    }
    
    if (grafo->matriz_adjacencia[indice_origem][indice_destino] != 0) {
        printf("Já existe um voo de %s para %s.\n", origem, destino);
        return false;
    }
    
    grafo->matriz_adjacencia[indice_origem][indice_destino] = numero_voo;
    printf("Voo %d de %s para %s cadastrado com sucesso.\n", numero_voo, origem, destino);
    return true;
}

bool remover_voo(GrafoAeroportos *grafo, const char *origem, const char *destino, int numero_voo) {
    int indice_origem = encontrar_indice_aeroporto(grafo, origem);
    int indice_destino = encontrar_indice_aeroporto(grafo, destino);
    
    if (indice_origem == -1) {
        printf("Aeroporto de origem %s não encontrado.\n", origem);
        return false;
    }
    
    if (indice_destino == -1) {
        printf("Aeroporto de destino %s não encontrado.\n", destino);
        return false;
    }
    
    if (grafo->matriz_adjacencia[indice_origem][indice_destino] != numero_voo) {
        printf("Voo %d de %s para %s não encontrado.\n", numero_voo, origem, destino);
        return false;
    }
    
    grafo->matriz_adjacencia[indice_origem][indice_destino] = 0;
    printf("Voo %d de %s para %s removido com sucesso.\n", numero_voo, origem, destino);
    return true;
}

void listar_voos_saida(GrafoAeroportos *grafo, const char *codigo_aeroporto) {
    int indice = encontrar_indice_aeroporto(grafo, codigo_aeroporto);
    
    if (indice == -1) {
        printf("Aeroporto %s não encontrado.\n", codigo_aeroporto);
        return;
    }
    
    printf("Voos saindo de %s (%s):\n", grafo->aeroportos[indice].codigo, grafo->aeroportos[indice].cidade);
    bool encontrou = false;
    
    for (int i = 0; i < grafo->num_aeroportos; i++) {
        if (grafo->matriz_adjacencia[indice][i] != 0) {
            printf("- Voo %d para %s (%s)\n", 
                   grafo->matriz_adjacencia[indice][i],
                   grafo->aeroportos[i].codigo,
                   grafo->aeroportos[i].cidade);
            encontrou = true;
        }
    }
    
    if (!encontrou) {
        printf("Nenhum voo saindo deste aeroporto.\n");
    }
}

static void dfs_trajetos(GrafoAeroportos *grafo, int origem, int destino, bool *visitados, int *caminho, int posicao) {
    visitados[origem] = true;
    caminho[posicao] = origem;
    posicao++;
    
    if (origem == destino) {
        for (int i = 0; i < posicao; i++) {
            printf("%s", grafo->aeroportos[caminho[i]].codigo);
            if (i < posicao - 1) {
                printf(" -> ");
            }
        }
        printf("\n");
    } else {
        for (int i = 0; i < grafo->num_aeroportos; i++) {
            if (grafo->matriz_adjacencia[origem][i] != 0 && !visitados[i]) {
                dfs_trajetos(grafo, i, destino, visitados, caminho, posicao);
            }
        }
    }
    
    posicao--;
    visitados[origem] = false;
}

void listar_trajetos(GrafoAeroportos *grafo, const char *origem, const char *destino) {
    int indice_origem = encontrar_indice_aeroporto(grafo, origem);
    int indice_destino = encontrar_indice_aeroporto(grafo, destino);
    
    if (indice_origem == -1) {
        printf("Aeroporto de origem %s não encontrado.\n", origem);
        return;
    }
    
    if (indice_destino == -1) {
        printf("Aeroporto de destino %s não encontrado.\n", destino);
        return;
    }
    
    printf("Trajetos de %s para %s:\n", origem, destino);
    
    bool *visitados = (bool*)calloc(grafo->num_aeroportos, sizeof(bool));
    int *caminho = (int*)malloc(grafo->num_aeroportos * sizeof(int));
    
    if (!visitados || !caminho) {
        printf("Erro de memória.\n");
        free(visitados);
        free(caminho);
        return;
    }
    
    dfs_trajetos(grafo, indice_origem, indice_destino, visitados, caminho, 0);
    
    free(visitados);
    free(caminho);
}