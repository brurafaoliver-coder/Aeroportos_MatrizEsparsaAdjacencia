#include "airport_graph.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ═══════════════════════════════════════════════════════════════════════════
   Inicialização / destruição
   ═══════════════════════════════════════════════════════════════════════════ */

int ag_init(AirportGraph *g) {
    if (!g) return 0;
    if (!av_init(&g->airports, 8)) return 0;
    g->matrix = sm_create(sizeof(Flight));
    if (!g->matrix) { av_destroy(&g->airports); return 0; }
    return 1;
}

void ag_destroy(AirportGraph *g) {
    if (!g) return;
    av_destroy(&g->airports);
    sm_destroy(g->matrix);
    g->matrix = NULL;
}

/* ═══════════════════════════════════════════════════════════════════════════
   Operação 1 – Cadastrar aeroporto
   ═══════════════════════════════════════════════════════════════════════════ */

int ag_add_airport(AirportGraph *g, const char *code, const char *city) {
    if (!g || !code || !city) {
        printf("[ERRO] Parâmetros inválidos.\n");
        return 0;
    }
    int idx = av_add(&g->airports, code, city);
    if (idx < 0) {
        printf("[ERRO] Aeroporto '%s' já cadastrado ou falha de memória.\n", code);
        return 0;
    }
    printf("[OK] Aeroporto %s (%s) cadastrado no índice %d.\n", code, city, idx);
    return 1;
}

/* ═══════════════════════════════════════════════════════════════════════════
   Operação 2 – Cadastrar voo
   ═══════════════════════════════════════════════════════════════════════════ */

int ag_add_flight(AirportGraph *g,
                  const char *origin_code,
                  const char *dest_code,
                  int flight_number) {
    if (!g || !origin_code || !dest_code) {
        printf("[ERRO] Parâmetros inválidos.\n");
        return 0;
    }
    int ori = av_find_by_code(&g->airports, origin_code);
    int dst = av_find_by_code(&g->airports, dest_code);

    if (ori < 0) { printf("[ERRO] Aeroporto de origem '%s' não encontrado.\n", origin_code); return 0; }
    if (dst < 0) { printf("[ERRO] Aeroporto de destino '%s' não encontrado.\n", dest_code);  return 0; }
    if (ori == dst) { printf("[ERRO] Origem e destino não podem ser o mesmo aeroporto.\n"); return 0; }

    /* Verifica se já existe um voo nesta rota (aresta já existente) */
    if (sm_get(g->matrix, ori, dst)) {
        printf("[ERRO] Já existe um voo de %s para %s.\n", origin_code, dest_code);
        return 0;
    }

    /* Verifica se número de voo já existe na malha (busca eficiente na matriz esparsa) */
    for (RowNode *r = g->matrix->rows; r; r = r->next) {
        for (ColNode *c = r->cols; c; c = c->next) {
            if (((Flight *)c->data)->flight_number == flight_number) {
                printf("[ERRO] Voo %d já cadastrado.\n", flight_number);
                return 0;
            }
        }
    }

    Flight fl = { .flight_number = flight_number };
    if (!sm_set(g->matrix, ori, dst, &fl)) {
        printf("[ERRO] Falha ao inserir voo na matriz.\n");
        return 0;
    }
    printf("[OK] Voo %d cadastrado: %s → %s.\n",
           flight_number, origin_code, dest_code);
    return 1;
}

/* ═══════════════════════════════════════════════════════════════════════════
   Operação 3 – Remover voo pelo número
   ═══════════════════════════════════════════════════════════════════════════ */

int ag_remove_flight(AirportGraph *g, int flight_number) {
    if (!g) return 0;
    /* Busca o voo percorrendo apenas as conexões existentes */
    for (RowNode *r = g->matrix->rows; r; r = r->next) {
        for (ColNode *c = r->cols; c; c = c->next) {
            if (((Flight *)c->data)->flight_number == flight_number) {
                int ori_idx = r->row;
                int dst_idx = c->col;
                sm_remove(g->matrix, ori_idx, dst_idx);
                printf("[OK] Voo %d removido (%s → %s).\n",
                       flight_number,
                       g->airports.data[ori_idx].code,
                       g->airports.data[dst_idx].code);
                return 1;
            }
        }
    }
    printf("[ERRO] Voo %d não encontrado.\n", flight_number);
    return 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
   Operação 4 – Listar voos que partem de um aeroporto
   ═══════════════════════════════════════════════════════════════════════════ */

typedef struct { AirportGraph *g; } ListCtx;

static void list_cb(int col, void *data, void *user_data) {
    ListCtx *ctx = user_data;
    Flight  *f   = data;
    printf("  Voo %-6d → %s (%s)\n",
           f->flight_number,
           ctx->g->airports.data[col].city,
           ctx->g->airports.data[col].code);
}

void ag_list_flights(AirportGraph *g, const char *origin_code) {
    if (!g || !origin_code) return;
    int ori = av_find_by_code(&g->airports, origin_code);
    if (ori < 0) { printf("[ERRO] Aeroporto '%s' não encontrado.\n", origin_code); return; }

    printf("\nVoos partindo de %s (%s):\n",
           g->airports.data[ori].city, origin_code);

    ListCtx ctx = { g };
    sm_foreach_col(g->matrix, ori, list_cb, &ctx);
    printf("\n");
}

/* ═══════════════════════════════════════════════════════════════════════════
   Operação 5 – Listar todos os trajetos (DFS recursiva)
   ═══════════════════════════════════════════════════════════════════════════ */

#define MAX_AIRPORTS 256

typedef struct {
    AirportGraph *g;
    int  dest;
    int  visited[MAX_AIRPORTS];
    int  path[MAX_AIRPORTS];
    int  path_len;
    int  found;
} PathCtx;

static void dfs(PathCtx *ctx, int current);

typedef struct { PathCtx *ctx; } DfsCallbackCtx;

static void dfs_cb(int col, void *data, void *user_data) {
    (void)data;
    DfsCallbackCtx *dc = user_data;
    if (col >= 0 && col < MAX_AIRPORTS && !dc->ctx->visited[col])
        dfs(dc->ctx, col);
}

static void dfs(PathCtx *ctx, int current) {
    int n = ctx->g->airports.size;
    if (current < 0 || current >= n || current >= MAX_AIRPORTS) return;

    ctx->visited[current] = 1;
    ctx->path[ctx->path_len++] = current;

    if (current == ctx->dest) {
        /* Imprime trajeto encontrado */
        ctx->found = 1;
        printf("  Trajeto: ");
        for (int i = 0; i < ctx->path_len; i++) {
            int idx = ctx->path[i];
            printf("%s(%s)", ctx->g->airports.data[idx].code,
                             ctx->g->airports.data[idx].city);
            if (i < ctx->path_len - 1) printf(" → ");
        }
        printf("\n");
    } else {
        DfsCallbackCtx dc = { ctx };
        sm_foreach_col(ctx->g->matrix, current, dfs_cb, &dc);
    }

    ctx->path_len--;
    ctx->visited[current] = 0;
}

void ag_find_paths(AirportGraph *g,
                   const char *origin_code,
                   const char *dest_code) {
    if (!g || !origin_code || !dest_code) return;

    int ori = av_find_by_code(&g->airports, origin_code);
    int dst = av_find_by_code(&g->airports, dest_code);

    if (ori < 0) { printf("[ERRO] Aeroporto de origem '%s' não encontrado.\n", origin_code); return; }
    if (dst < 0) { printf("[ERRO] Aeroporto de destino '%s' não encontrado.\n", dest_code);  return; }
    if (ori == dst) { printf("[ERRO] Origem e destino são o mesmo aeroporto.\n"); return; }

    printf("\nTrajetos de %s → %s:\n", origin_code, dest_code);

    PathCtx ctx = {0};
    ctx.g    = g;
    ctx.dest = dst;

    dfs(&ctx, ori);

    if (!ctx.found)
        printf("  Nenhum trajeto encontrado.\n");
    printf("\n");
}
