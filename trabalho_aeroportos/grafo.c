#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* aqui estamos criando o grafo de aeroporto
esse grafo tem a matriz de adjacencia ja colocada nele que tem as funções de
adicionar aeroporto, adicionar voo, remover voo, listar voos de um aeroporto 
e listar trajetos entre dois aeroportos.
*/
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
/* aqui nós estamos destruindo o grafo,  esse "destruir"
seria liberar a memória do grafo, ou seja mesma coisa que free
*/
void destruir_grafo(GrafoAeroportos *grafo) {
    if (!grafo) return;
    
    for (int i = 0; i < grafo->capacidade; i++) {
        free(grafo->matriz_adjacencia[i]);
    }
    free(grafo->matriz_adjacencia);
    free(grafo->aeroportos);
    free(grafo);
}
// aqui usamos o strcmp para comparar o código do aeroporto com o 
// código que estamos procurando
int encontrar_indice_aeroporto(GrafoAeroportos *grafo, const char *codigo) {
    for (int i = 0; i < grafo->num_aeroportos; i++) {
        if (strcmp(grafo->aeroportos[i].codigo, codigo) == 0) {
            return i;
        }
    }
    return -1;
}
// aqui adicionamos o aeroporto
// e nessa função usamos o strncpy para copiar a string do código e da cidade para o aeroporto
// depois disso, incrementamos o número de aeroportos e imprimimos uma mensagem de sucesso
// com base no que foi digitado 
bool adicionar_aeroporto(GrafoAeroportos *grafo, const char *codigo, const char *cidade) {
    if (grafo->num_aeroportos >= grafo->capacidade) {
        printf("Capacidade maxima de aeroportos atingida.\n");
        return false;
        // se o num de aeroportos que tem no grafo for menor que a capacidade 
        // mas aqui só temos a validação de problemas
    }
    
    if (encontrar_indice_aeroporto(grafo, codigo) != -1) {
        printf("Aeroporto com código %s ja existe.\n", codigo);
        return false;
    }
    //strncpy é string copy
    // o código_size é -1 pois precisamos adicionar o nulo no final para evitar bugs
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
    
    grafo->matriz_adjacencia[indice_origem][indice_destino] = numero_voo;
    printf("Voo %d de %s para %s cadastrado com sucesso.\n", numero_voo, origem, destino);
    return true;
}

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
    
    grafo->matriz_adjacencia[indice_origem][indice_destino] = 0;
    printf("Voo %d de %s para %s removido com sucesso.\n", numero_voo, origem, destino);
    return true;
}

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
/*Aqui temos a função de listar trajetos,usa o grafo para achar o ponto de origem e destino, e ai 
encontra o trajeto entre o ponto A e B. dfs é o busca por profundidade, ir até o final e enocontrar 
o que tiver que encontrar busca recursiva*/
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
        printf("Aeroporto de origem %s nao encontrado.\n", origem);
        return;
    }
    
    if (indice_destino == -1) {
        printf("Aeroporto de destino %s nao encontrado.\n", destino);
        return;
    }
    
    printf("Trajetos de %s para %s:\n", origem, destino);

    bool *visitados = (bool*)calloc(grafo->num_aeroportos, sizeof(bool));
    int *caminho = (int*)malloc(grafo->num_aeroportos * sizeof(int));
    
    if (!visitados || !caminho) {
        printf("Erro de memoria.\n");
        free(visitados);
        free(caminho);
        return;
    }
    
    dfs_trajetos(grafo, indice_origem, indice_destino, visitados, caminho, 0);
    
    free(visitados);
    free(caminho);
}