#include "menu.h"
#include <stdio.h>

void menu(GrafoAeroportos *grafo) {
    int opcao;
    char codigo[CODIGO_SIZE], cidade[CIDADE_SIZE];
    char origem[CODIGO_SIZE], destino[CODIGO_SIZE];
    int numero_voo;
    
    do {
        printf("\n=== MENU ===\n");
        printf("1. Cadastrar novo aeroporto\n");
        printf("2. Cadastrar voo\n");
        printf("3. Remover voo\n");
        printf("4. Listar voos que saem de um aeroporto\n");
        printf("5. Listar trajetos entre dois aeroportos\n");
        printf("0. Sair\n");
        printf("Escolha uma opcaoo: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                printf("Codigo do aeroporto (3 letras): ");
                scanf("%3s", codigo);
                printf("Cidade: ");
                scanf(" %[^\n]", cidade);
                adicionar_aeroporto(grafo, codigo, cidade);
                break;
                
            case 2:
                printf("Codigo do aeroporto de origem: ");
                scanf("%3s", origem);
                printf("Codigo do aeroporto de destino: ");
                scanf("%3s", destino);
                printf("Numero do voo: ");
                scanf("%d", &numero_voo);
                adicionar_voo(grafo, origem, destino, numero_voo);
                break;
                
            case 3:
                printf("Codigo do aeroporto de origem: ");
                scanf("%3s", origem);
                printf("Codigo do aeroporto de destino: ");
                scanf("%3s", destino);
                printf("NNumero do voo a ser removido: ");
                scanf("%d", &numero_voo);
                remover_voo(grafo, origem, destino, numero_voo);
                break;
                
            case 4:
                printf("Codigo do aeroporto: ");
                scanf("%3s", codigo);
                listar_voos_saida(grafo, codigo);
                break;
                
            case 5:
                printf("Codigo do aeroporto de origem: ");
                scanf("%3s", origem);
                printf("Codigo do aeroporto de destino: ");
                scanf("%3s", destino);
                listar_trajetos(grafo, origem, destino);
                break;
                
            case 0:
                printf("Saindo...\n");
                break;
                
            default:
                printf("Opção invalida.\n");
        }
    } while (opcao != 0);
}