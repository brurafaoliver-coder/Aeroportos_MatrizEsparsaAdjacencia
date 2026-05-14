#ifndef AIRPORT_GRAPH_H
#define AIRPORT_GRAPH_H

#include "airport_vector.h"
#include "sparse_matrix.h"

/* ─── Dado armazenado em cada aresta da matriz (um voo) ──────────────────── */
typedef struct {
    int flight_number; /* número do voo */
} Flight;

/* ─── Grafo principal ────────────────────────────────────────────────────── */
typedef struct {
    AirportVector airports; /* vetor dinâmico de aeroportos (vértices) */
    SparseMatrix  *matrix;  /* matriz esparsa de adjacência  (arestas)  */
} AirportGraph;

/* ─── Interface pública ──────────────────────────────────────────────────── */

/** Inicializa o grafo. Retorna 1 em sucesso, 0 em falha. */
int ag_init(AirportGraph *g);

/** Libera todos os recursos do grafo. */
void ag_destroy(AirportGraph *g);

/* ── Operação 1: Cadastrar aeroporto ─────────────────────────────────────── */
int ag_add_airport(AirportGraph *g, const char *code, const char *city);

/* ── Operação 2: Cadastrar voo ───────────────────────────────────────────── */
int ag_add_flight(AirportGraph *g,
                  const char *origin_code,
                  const char *dest_code,
                  int flight_number);

/* ── Operação 3: Remover voo pelo número ─────────────────────────────────── */
int ag_remove_flight(AirportGraph *g, int flight_number);

/* ── Operação 4: Listar voos de um aeroporto ─────────────────────────────── */
void ag_list_flights(AirportGraph *g, const char *origin_code);

/* ── Operação 5: Listar trajetos entre dois aeroportos (DFS) ─────────────── */
void ag_find_paths(AirportGraph *g,
                   const char *origin_code,
                   const char *dest_code);

#endif /* AIRPORT_GRAPH_H */
