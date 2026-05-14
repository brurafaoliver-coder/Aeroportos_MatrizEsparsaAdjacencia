#include "airport_vector.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int av_init(AirportVector *v, int initial_capacity) {
    if (!v || initial_capacity <= 0) return 0;
    v->data = malloc(sizeof(Airport) * initial_capacity);
    if (!v->data) return 0;
    v->size     = 0;
    v->capacity = initial_capacity;
    return 1;
}

void av_destroy(AirportVector *v) {
    if (!v) return;
    free(v->data);
    v->data     = NULL;
    v->size     = 0;
    v->capacity = 0;
}

/* Dobra a capacidade quando necessário */
static int av_grow(AirportVector *v) {
    int new_cap = v->capacity * 2;
    Airport *tmp = realloc(v->data, sizeof(Airport) * new_cap);
    if (!tmp) return 0;
    v->data     = tmp;
    v->capacity = new_cap;
    return 1;
}

int av_add(AirportVector *v, const char *code, const char *city) {
    if (!v || !code || !city) return -1;

    /* Verifica se código já existe */
    if (av_find_by_code(v, code) >= 0) return -1;

    if (v->size == v->capacity && !av_grow(v)) return -1;

    strncpy(v->data[v->size].code, code, AIRPORT_CODE_LEN - 1);
    v->data[v->size].code[AIRPORT_CODE_LEN - 1] = '\0';
    strncpy(v->data[v->size].city, city, AIRPORT_CITY_LEN - 1);
    v->data[v->size].city[AIRPORT_CITY_LEN - 1] = '\0';

    return v->size++;
}

int av_find_by_code(const AirportVector *v, const char *code) {
    if (!v || !code) return -1;
    for (int i = 0; i < v->size; i++) {
        if (strncmp(v->data[i].code, code, AIRPORT_CODE_LEN) == 0)
            return i;
    }
    return -1;
}

int av_remove(AirportVector *v, int index) {
    if (!v || index < 0 || index >= v->size) return 0;
    /* Desloca elementos para preencher a lacuna */
    for (int i = index; i < v->size - 1; i++)
        v->data[i] = v->data[i + 1];
    v->size--;

    /* Encolhe se uso cair abaixo de 25% */
    if (v->size > 0 && v->size <= v->capacity / 4 && v->capacity > 4) {
        int new_cap = v->capacity / 2;
        Airport *tmp = realloc(v->data, sizeof(Airport) * new_cap);
        if (tmp) { v->data = tmp; v->capacity = new_cap; }
    }
    return 1;
}

void av_print(const AirportVector *v) {
    if (!v || v->size == 0) {
        printf("Nenhum aeroporto cadastrado.\n");
        return;
    }
    printf("\n%-5s %-6s %s\n", "Idx", "Código", "Cidade");
    printf("%-5s %-6s %s\n", "---", "------", "------");
    for (int i = 0; i < v->size; i++)
        printf("%-5d %-6s %s\n", i, v->data[i].code, v->data[i].city);
}
