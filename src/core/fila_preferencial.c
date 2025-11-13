#include "fila_preferencial.h"
#include <stdio.h>
#include <stdlib.h>

static int data_antes_cmp(const struct tm *a, const struct tm *b) {
    if (a->tm_year != b->tm_year) return (a->tm_year < b->tm_year) ? -1 : 1;
    if (a->tm_mon  != b->tm_mon)  return (a->tm_mon  < b->tm_mon)  ? -1 : 1;
    if (a->tm_mday != b->tm_mday) return (a->tm_mday < b->tm_mday) ? -1 : 1;
    if (a->tm_hour != b->tm_hour) return (a->tm_hour < b->tm_hour) ? -1 : 1;
    if (a->tm_min  != b->tm_min)  return (a->tm_min  < b->tm_min)  ? -1 : 1;
    if (a->tm_sec  != b->tm_sec)  return (a->tm_sec  < b->tm_sec)  ? -1 : 1;
    return 0;
}

/* Comparator para min-heap que faz o "melhor" (maior prioridade, data mais antiga) ser o menor */
static int cmp_chamado_max(const void *pa, const void *pb) {
    const Chamado *a = (const Chamado*)pa;
    const Chamado *b = (const Chamado*)pb;
    if (a->prioridade != b->prioridade) {
        return (a->prioridade > b->prioridade) ? -1 : 1;
    }
    return data_antes_cmp(&a->data_hora, &b->data_hora);
}

FilaPrioridadeMaxima* criar_fila_prioridade_maxima(int capacidade) {
    if (capacidade <= 0) capacidade = 8;
    FilaPrioridadeMaxima *fila = (FilaPrioridadeMaxima*)malloc(sizeof(FilaPrioridadeMaxima));
    if (!fila) return NULL;
    fila->h = heap_create((size_t)capacidade, cmp_chamado_max);
    if (!fila->h) {
        free(fila);
        return NULL;
    }
    return fila;
}

void inserir(FilaPrioridadeMaxima *fila, Chamado valor) {
    if (!fila || !fila->h) return;
    Chamado *ptr = (Chamado*)malloc(sizeof(Chamado));
    if (!ptr) return;
    *ptr = valor;
    if (!heap_push(fila->h, ptr)) {
        free(ptr);
        printf("Overflow do heap\n");
    }
}

Chamado extrair_maximo(FilaPrioridadeMaxima *fila) {
    Chamado vazio;
    vazio.descricao[0] = '\0';
    vazio.prioridade = 0;
    vazio.nome_pessoa[0] = '\0';
    vazio.data_hora = (struct tm){0};
    if (!fila || !fila->h || heap_is_empty(fila->h)) {
        printf("Underflow do heap\n");
        return vazio;
    }
    Chamado *top = (Chamado*)heap_pop(fila->h);
    Chamado result = vazio;
    if (top) {
        result = *top;
        free(top);
    }
    return result;
}

void liberar_fila(FilaPrioridadeMaxima *fila) {
    if (!fila) return;
    if (fila->h) {
        while (!heap_is_empty(fila->h)) {
            Chamado *ptr = (Chamado*)heap_pop(fila->h);
            free(ptr);
        }
        heap_destroy(fila->h);
    }
    free(fila);
}
