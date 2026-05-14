#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "airport_graph.h"

/* ─── Utilitários de leitura ─────────────────────────────────────────────── */

static void read_line(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buf, size, stdin)) {
        buf[strcspn(buf, "\n")] = '\0'; /* remove '\n' */
    }
}

static int read_int(const char *prompt) {
    char buf[32];
    read_line(prompt, buf, sizeof(buf));
    return atoi(buf);
}

/* ─── Menu ───────────────────────────────────────────────────────────────── */

static void print_menu(void) {
    printf("\n╔══════════════════════════════════╗\n");
    printf("║  Sistema de Malha Aérea – ANAC   ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║ 1. Cadastrar aeroporto           ║\n");
    printf("║ 2. Cadastrar voo                 ║\n");
    printf("║ 3. Remover voo                   ║\n");
    printf("║ 4. Listar voos de um aeroporto   ║\n");
    printf("║ 5. Buscar trajetos               ║\n");
    printf("║ 6. Listar todos os aeroportos    ║\n");
    printf("║ 0. Sair                          ║\n");
    printf("╚══════════════════════════════════╝\n");
    printf("Opção: ");
    fflush(stdout);
}

/* ─── Carga inicial de dados de exemplo ─────────────────────────────────── */

static void load_sample_data(AirportGraph *g) {
    printf("\n--- Carregando dados de exemplo ---\n");
    ag_add_airport(g, "BSB", "Brasília");
    ag_add_airport(g, "CNF", "Belo Horizonte");
    ag_add_airport(g, "GIG", "Rio de Janeiro");
    ag_add_airport(g, "GRU", "São Paulo");
    ag_add_airport(g, "SSA", "Salvador");

    /* Voos conforme o diagrama do trabalho */
    ag_add_flight(g, "BSB", "SSA", 107);
    ag_add_flight(g, "CNF", "GIG", 555);
    ag_add_flight(g, "CNF", "GRU", 101);
    ag_add_flight(g, "CNF", "SSA", 214);
    ag_add_flight(g, "GIG", "CNF", 554);
    ag_add_flight(g, "GIG", "GRU", 90);
    ag_add_flight(g, "GRU", "BSB", 50);
    ag_add_flight(g, "GRU", "CNF", 102);
    ag_add_flight(g, "GRU", "GIG", 89);
    ag_add_flight(g, "SSA", "CNF", 215);
    printf("-----------------------------------\n");
}

/* ─── main ───────────────────────────────────────────────────────────────── */

int main(void) {
    AirportGraph g;
    if (!ag_init(&g)) {
        fprintf(stderr, "Falha ao inicializar o grafo.\n");
        return EXIT_FAILURE;
    }

    load_sample_data(&g);

    int option;
    char code1[8], code2[8], city[64];
    int flight_num;

    do {
        print_menu();
        option = read_int("");

        switch (option) {
            case 1:
                read_line("Código IATA (ex.: GRU): ", code1, sizeof(code1));
                read_line("Nome da cidade:          ", city,  sizeof(city));
                ag_add_airport(&g, code1, city);
                break;

            case 2:
                read_line("Código origem:  ", code1, sizeof(code1));
                read_line("Código destino: ", code2, sizeof(code2));
                flight_num = read_int("Número do voo:  ");
                ag_add_flight(&g, code1, code2, flight_num);
                break;

            case 3:
                flight_num = read_int("Número do voo a remover: ");
                ag_remove_flight(&g, flight_num);
                break;

            case 4:
                read_line("Código do aeroporto de origem: ", code1, sizeof(code1));
                ag_list_flights(&g, code1);
                break;

            case 5:
                read_line("Código de origem:  ", code1, sizeof(code1));
                read_line("Código de destino: ", code2, sizeof(code2));
                ag_find_paths(&g, code1, code2);
                break;

            case 6:
                av_print(&g.airports);
                break;

            case 0:
                printf("Encerrando... Bom trabalho!\n");
                break;

            default:
                printf("[AVISO] Opção inválida. Tente novamente.\n");
        }

    } while (option != 0);

    ag_destroy(&g);
    return EXIT_SUCCESS;
}
