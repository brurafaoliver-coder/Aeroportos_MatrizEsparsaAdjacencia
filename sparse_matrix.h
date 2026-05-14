#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <stdlib.h>

/* ─── Nó da lista encadeada (coluna) ─────────────────────────────────────── */
typedef struct ColNode {
    int col;            /* índice da coluna (aeroporto destino) */
    void *data;         /* dado genérico armazenado na célula   */
    struct ColNode *next;
} ColNode;

/* ─── Nó da lista encadeada (linha) ──────────────────────────────────────── */
typedef struct RowNode {
    int row;            /* índice da linha (aeroporto origem)   */
    ColNode *cols;      /* lista de colunas desta linha         */
    struct RowNode *next;
} RowNode;

/* ─── Matriz esparsa ─────────────────────────────────────────────────────── */
typedef struct {
    RowNode *rows;      /* lista de linhas com dados            */
    size_t data_size;   /* tamanho do tipo de dado genérico     */
} SparseMatrix;

/* ─── Interface pública ──────────────────────────────────────────────────── */

/**
 * Cria uma matriz esparsa para elementos de tamanho data_size bytes.
 * Retorna ponteiro para a matriz ou NULL em caso de falha.
 */
SparseMatrix *sm_create(size_t data_size);

/**
 * Libera toda a memória da matriz.
 */
void sm_destroy(SparseMatrix *m);

/**
 * Insere (ou substitui) o valor apontado por data na posição (row, col).
 * Retorna 1 em sucesso, 0 em falha.
 */
int sm_set(SparseMatrix *m, int row, int col, const void *data);

/**
 * Obtém o ponteiro para o dado na posição (row, col).
 * Retorna NULL se a célula estiver vazia.
 */
void *sm_get(SparseMatrix *m, int row, int col);

/**
 * Remove a célula (row, col).
 * Retorna 1 se removeu, 0 se não existia.
 */
int sm_remove(SparseMatrix *m, int row, int col);

/**
 * Itera sobre todas as colunas de uma linha.
 * Chama callback(col, data, user_data) para cada célula presente.
 */
void sm_foreach_col(SparseMatrix *m, int row,
                    void (*callback)(int col, void *data, void *user_data),
                    void *user_data);

#endif /* SPARSE_MATRIX_H */
