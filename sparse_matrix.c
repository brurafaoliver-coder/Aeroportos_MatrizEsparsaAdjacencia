#include "sparse_matrix.h"
#include <string.h>
#include <stdio.h>

/* ─── Helpers internos ───────────────────────────────────────────────────── */

static RowNode *find_row(SparseMatrix *m, int row) {
    RowNode *r = m->rows;
    while (r && r->row != row) r = r->next;
    return r;
}

static RowNode *get_or_create_row(SparseMatrix *m, int row) {
    RowNode **pp = &m->rows;
    while (*pp && (*pp)->row < row) pp = &(*pp)->next;
    if (*pp && (*pp)->row == row) return *pp;

    RowNode *nr = malloc(sizeof(RowNode));
    if (!nr) return NULL;
    nr->row  = row;
    nr->cols = NULL;
    nr->next = *pp;
    *pp = nr;
    return nr;
}

/* ─── API pública ────────────────────────────────────────────────────────── */

SparseMatrix *sm_create(size_t data_size) {
    SparseMatrix *m = malloc(sizeof(SparseMatrix));
    if (!m) return NULL;
    m->rows      = NULL;
    m->data_size = data_size;
    return m;
}

void sm_destroy(SparseMatrix *m) {
    if (!m) return;
    RowNode *r = m->rows;
    while (r) {
        ColNode *c = r->cols;
        while (c) {
            ColNode *tmp = c->next;
            free(c->data);
            free(c);
            c = tmp;
        }
        RowNode *tmp = r->next;
        free(r);
        r = tmp;
    }
    free(m);
}

int sm_set(SparseMatrix *m, int row, int col, const void *data) {
    if (!m || !data) return 0;

    RowNode *r = get_or_create_row(m, row);
    if (!r) return 0;

    /* Procura (ou cria) a coluna */
    ColNode **pp = &r->cols;
    while (*pp && (*pp)->col < col) pp = &(*pp)->next;

    if (*pp && (*pp)->col == col) {
        /* Substitui dado existente */
        memcpy((*pp)->data, data, m->data_size);
        return 1;
    }

    /* Cria novo nó de coluna */
    ColNode *nc = malloc(sizeof(ColNode));
    if (!nc) return 0;
    nc->data = malloc(m->data_size);
    if (!nc->data) { free(nc); return 0; }
    memcpy(nc->data, data, m->data_size);
    nc->col  = col;
    nc->next = *pp;
    *pp = nc;
    return 1;
}

void *sm_get(SparseMatrix *m, int row, int col) {
    if (!m) return NULL;
    RowNode *r = find_row(m, row);
    if (!r) return NULL;
    ColNode *c = r->cols;
    while (c && c->col != col) c = c->next;
    return c ? c->data : NULL;
}

int sm_remove(SparseMatrix *m, int row, int col) {
    if (!m) return 0;
    /* Localiza a linha mantendo o ponteiro anterior para permitir remoção do nó da linha */
    RowNode **ppr = &m->rows;
    while (*ppr && (*ppr)->row != row) ppr = &(*ppr)->next;
    if (!*ppr) return 0;

    RowNode *r = *ppr;
    ColNode **ppc = &r->cols;
    while (*ppc && (*ppc)->col != col) ppc = &(*ppc)->next;
    if (!*ppc) return 0;

    ColNode *del = *ppc;
    *ppc = del->next;
    free(del->data);
    free(del);

    /* Se a linha ficou sem colunas, removemos o nó da linha também */
    if (r->cols == NULL) {
        *ppr = r->next;
        free(r);
    }
    return 1;
}

void sm_foreach_col(SparseMatrix *m, int row,
                    void (*callback)(int col, void *data, void *user_data),
                    void *user_data) {
    if (!m || !callback) return;
    RowNode *r = find_row(m, row);
    if (!r) return;
    ColNode *c = r->cols;
    while (c) {
        callback(c->col, c->data, user_data);
        c = c->next;
    }
}
