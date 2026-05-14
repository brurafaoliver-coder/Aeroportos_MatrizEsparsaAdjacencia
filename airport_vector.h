#ifndef AIRPORT_VECTOR_H
#define AIRPORT_VECTOR_H

#define AIRPORT_CODE_LEN  4   /* ex.: "GRU\0" */
#define AIRPORT_CITY_LEN  64

/* ─── Estrutura de um aeroporto ──────────────────────────────────────────── */
typedef struct {
    char code[AIRPORT_CODE_LEN]; /* código IATA  (ex.: "GRU") */
    char city[AIRPORT_CITY_LEN]; /* nome da cidade (ex.: "São Paulo") */
} Airport;

/* ─── Vetor dinâmico de aeroportos ──────────────────────────────────────── */
typedef struct {
    Airport *data;
    int size;       /* número de aeroportos cadastrados */
    int capacity;   /* capacidade atual do vetor        */
} AirportVector;

/* ─── Interface pública ──────────────────────────────────────────────────── */

/**
 * Inicializa o vetor com capacidade inicial.
 * Retorna 1 em sucesso, 0 em falha.
 */
int av_init(AirportVector *v, int initial_capacity);

/**
 * Libera a memória do vetor.
 */
void av_destroy(AirportVector *v);

/**
 * Adiciona um aeroporto ao vetor.
 * Retorna o índice do aeroporto inserido, ou -1 em falha.
 */
int av_add(AirportVector *v, const char *code, const char *city);

/**
 * Retorna o índice do aeroporto com o código dado, ou -1 se não encontrado.
 */
int av_find_by_code(const AirportVector *v, const char *code);

/**
 * Remove o aeroporto do índice dado (compacta o vetor).
 * Retorna 1 em sucesso, 0 em falha.
 */
int av_remove(AirportVector *v, int index);

/**
 * Imprime todos os aeroportos cadastrados.
 */
void av_print(const AirportVector *v);

#endif /* AIRPORT_VECTOR_H */
