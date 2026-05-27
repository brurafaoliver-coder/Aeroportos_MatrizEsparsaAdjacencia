#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "menu.h"

int main() {
    GrafoAeroportos *grafo = criar_grafo();
    
    adicionar_aeroporto(grafo, "CNF", "Belo Horizonte");
    adicionar_aeroporto(grafo, "BSB", "Brasilia");
    adicionar_aeroporto(grafo, "GIG", "Rio de Janeiro");
    adicionar_aeroporto(grafo, "SSA", "Salvador");
    adicionar_aeroporto(grafo, "GRU", "Sao Paulo");
    
    adicionar_voo(grafo, "BSB", "SSA", 107);
    adicionar_voo(grafo, "CNF", "GIG", 555);
    adicionar_voo(grafo, "CNF", "GRU", 101);
    adicionar_voo(grafo, "CNF", "SSA", 214);
    adicionar_voo(grafo, "GIG", "CNF", 554);
    adicionar_voo(grafo, "GIG", "GRU", 90);
    adicionar_voo(grafo, "GRU", "BSB", 50);
    adicionar_voo(grafo, "GRU", "CNF", 102);
    adicionar_voo(grafo, "GRU", "GIG", 89);
    adicionar_voo(grafo, "SSA", "CNF", 215);
    
    
    menu(grafo);
    
    destruir_grafo(grafo);
    return 0;
}