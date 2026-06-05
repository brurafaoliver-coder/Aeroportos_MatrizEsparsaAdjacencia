#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* função para criar o grafo de aeroportos */
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
/* função para destruir o grafo de aeroportos e liberar memória*/
void destruir_grafo(GrafoAeroportos *grafo) {
    if (!grafo) return;
    
    for (int i = 0; i < grafo->capacidade; i++) {
        free(grafo->matriz_adjacencia[i]);
    }
    free(grafo->matriz_adjacencia);
    free(grafo->aeroportos);
    free(grafo);
}
/*Usamos o strcmp para comparar o código do aeroporto com o código que estamos procurando*/
int encontrar_indice_aeroporto(GrafoAeroportos *grafo, const char *codigo) {
    for (int i = 0; i < grafo->num_aeroportos; i++) {
        if (strcmp(grafo->aeroportos[i].codigo, codigo) == 0) {
            return i;
        }
    }
    return -1;
}
/*na função de adicionar o aeroporto usamos o strncpy para copiar a string do código e da cidade para o aeroporto
depois disso, incrementamos o número de aeroportos e imprimimos uma mensagem de sucesso com base no que foi digitado*/
bool adicionar_aeroporto(GrafoAeroportos *grafo, const char *codigo, const char *cidade) {
    if (grafo->num_aeroportos >= grafo->capacidade) {
        printf("Capacidade maxima de aeroportos atingida.\n");
        return false;
    }
    
    if (encontrar_indice_aeroporto(grafo, codigo) != -1) {
        printf("Aeroporto com código %s ja existe.\n", codigo);
        return false;
    }
    // o código_size é -1 pois precisamos adicionar o nulo no final para evitar bugs
    strncpy(grafo->aeroportos[grafo->num_aeroportos].codigo, codigo, CODIGO_SIZE - 1);
    grafo->aeroportos[grafo->num_aeroportos].codigo[CODIGO_SIZE - 1] = '\0';
    
    strncpy(grafo->aeroportos[grafo->num_aeroportos].cidade, cidade, CIDADE_SIZE - 1);
    grafo->aeroportos[grafo->num_aeroportos].cidade[CIDADE_SIZE - 1] = '\0';
    
    grafo->num_aeroportos++;
    printf("Aeroporto %s - %s cadastrado com sucesso.\n", codigo, cidade);
    return true;
}
/*usamos bool para retornar se a função foi bem sucedida ou não, e usamos o encontrar_indice_aeroporto para 
verificar se os aeroportos de origem e destino existem antes de adicionar o voo. Se o voo já existir, também 
retornamos false. Caso contrário, adicionamos o número do voo na matriz de adjacência e retornamos true*/
bool adicionar_voo(GrafoAeroportos *grafo, const char *origem, const char *destino, int numero_voo) {
    int indice_origem = encontrar_indice_aeroporto(grafo, origem);
    int indice_destino = encontrar_indice_aeroporto(grafo, destino);
    
    if (indice_origem == -1) {
        printf("Aeroporto de origem %s nao encontrado.\n", origem);
        return false;
    }
    
    if (indice_destino == -1) {
        printf("Aeroporto de destino %s nao encontrado.\n", destino);
        return false;
    }
    
    if (grafo->matriz_adjacencia[indice_origem][indice_destino] != 0) {
        printf("Ja existe um voo de %s para %s.\n", origem, destino);
        return false;
    }
    /*usamos o número do voo para representar a existência do voo na matriz de adjacência, se for 
    diferente de 0, significa que já existe um voo entre esses aeroportos. Se for 0, significa que 
    não existe um voo e podemos adicionar o número do voo na matriz*/
    grafo->matriz_adjacencia[indice_origem][indice_destino] = numero_voo;
    printf("Voo %d de %s para %s cadastrado com sucesso.\n", numero_voo, origem, destino);
    return true;
}
/*usamos bool para retornar se a função foi bem sucedida ou não, e usamos o encontrar_indice_aeroporto para 
verificar se os aeroportos de origem e destino existem antes de remover o voo. Se o voo não existir, retornamos false. 
Caso contrário, removemos o voo da matriz de adjacência (definindo o valor para 0) e retornamos true*/
bool remover_voo(GrafoAeroportos *grafo, const char *origem, const char *destino, int numero_voo) {
    int indice_origem = encontrar_indice_aeroporto(grafo, origem);
    int indice_destino = encontrar_indice_aeroporto(grafo, destino);
    
    if (indice_origem == -1) {
        printf("Aeroporto de origem %s nao encontrado.\n", origem);
        return false;
    }
    
    if (indice_destino == -1) {
        printf("Aeroporto de destino %s nao encontrado.\n", destino);
        return false;
    }
    
    if (grafo->matriz_adjacencia[indice_origem][indice_destino] != numero_voo) {
        printf("Voo %d de %s para %s nao encontrado.\n", numero_voo, origem, destino);
        return false;
    }
    /*para remover o voo, basta definir o valor na matriz de adjacência para 0, indicando que não existe mais 
    um voo entre esses aeroportos*/
    grafo->matriz_adjacencia[indice_origem][indice_destino] = 0;
    printf("Voo %d de %s para %s removido com sucesso.\n", numero_voo, origem, destino);
    return true;
}
/*criamos a função void listar_voos_saida para listar os voos que saem de um aeroporto específico. 
Primeiro, encontramos o índice do aeroporto, se ele não fo encontrado exibimos uma mensagem de erro.  
se não percorremos a matriz para encontrar os voos que saem desse aeroporto e exibimos as 
informações dos voos encontrados. Se nenhum voo for encontrado, exibimos uma mensagem indicando isso*/
void listar_voos_saida(GrafoAeroportos *grafo, const char *codigo_aeroporto) {
    int indice = encontrar_indice_aeroporto(grafo, codigo_aeroporto);
    
    if (indice == -1) {
        printf("Aeroporto %s nao encontrado.\n", codigo_aeroporto);
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
/*criamos a função static void dfs_trajetos para realizar uma busca em profundidade (DFS) no grafo, 
procurando por trajetos entre o aeroporto de origem e o de destino*/
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
/*a função listar_trajetos é a função principal que será chamada para listar os trajetos entre dois aeroportos.*/
void listar_trajetos(GrafoAeroportos *grafo, const char *origem, const char *destino) {
    int indice_origem = encontrar_indice_aeroporto(grafo, origem);
    int indice_destino = encontrar_indice_aeroporto(grafo, destino);
    
    if (indice_origem == -1) {
        printf("Aeroporto de origem %s nao encontrado.\n", origem);
        return;
    }
    
    if (indice_destino == -1) {
        printf("Aeroporto de destino %s nao encontrado.\n", destino);
        return;
    }
    
    printf("Trajetos de %s para %s:\n", origem, destino);
    /*declaração de dois ponteiros simples que recebem blocos de memória alocados dinamicamente via calloc e malloc.
    Eles funcionam como vetores locais para controlar quais aeroportos já foram visitados e qual é o caminho atual 
    do algoritmo de busca (DFS)*/
    bool *visitados = (bool*)calloc(grafo->num_aeroportos, sizeof(bool));
    int *caminho = (int*)malloc(grafo->num_aeroportos * sizeof(int));
    
    if (!visitados || !caminho) {
        printf("Erro de memoria.\n");
        free(visitados);
        free(caminho);
        return;
    }
    /*chamamos a função dfs_trajetos para realizar a busca em profundidade e listar os trajetos 
    encontrados entre os aeroportos de origem e destino*/
    dfs_trajetos(grafo, indice_origem, indice_destino, visitados, caminho, 0);
    
    // Liberar memória que foi alocada para os arrays de visitados e caminho
    free(visitados);
    free(caminho);
}