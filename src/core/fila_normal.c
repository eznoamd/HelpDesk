#include "fila_normal.h"
#include <stdlib.h>
#include <stdio.h>

FilaNormal* criar_fila_normal(int capacidade) {
    if (capacidade <= 0) capacidade = 8;
    FilaNormal *fila = (FilaNormal*)malloc(sizeof(FilaNormal));
    if (!fila) return NULL;
    fila->q = fifo_create((size_t)capacidade);
    if (!fila->q) {
        free(fila);
        return NULL;
    }
    return fila;
}

int enfileirar(FilaNormal *fila, Chamado valor) {
    if (!fila || !fila->q) return 0;
    Chamado *ptr = (Chamado*)malloc(sizeof(Chamado));
    if (!ptr) return 0;
    *ptr = valor;
    if (!fifo_enqueue(fila->q, ptr)) {
        free(ptr);
        return 0;
    }
    return 1;
}

int desenfileirar(FilaNormal *fila, Chamado *out) {
    if (!fila || !fila->q || fifo_is_empty(fila->q)) return 0;
    Chamado *ptr = (Chamado*)fifo_dequeue(fila->q);
    if (!ptr) return 0;
    if (out) *out = *ptr;
    free(ptr);
    return 1;
}

int frente(FilaNormal *fila, Chamado *out) {
    if (!fila || !fila->q || fifo_is_empty(fila->q)) return 0;
    Chamado *ptr = (Chamado*)fifo_peek(fila->q);
    if (!ptr) return 0;
    if (out) *out = *ptr;
    return 1;
}

int fila_vazia(FilaNormal *fila) {
    if (!fila || !fila->q) return 1;
    return fifo_is_empty(fila->q);
}

void liberar_fila_normal(FilaNormal *fila) {
    if (!fila) return;
    if (fila->q) {
        while (!fifo_is_empty(fila->q)) {
            Chamado *ptr = (Chamado*)fifo_dequeue(fila->q);
            free(ptr);
        }
        fifo_destroy(fila->q);
    }
    free(fila);
}

void fila_normal_iterar(FilaNormal *fila, void (*cb)(const Chamado*, void*), void *ctx) {
    if (!fila || !fila->q || !cb) return;
    size_t n = fifo_size(fila->q);
    if (n == 0) return;
    Chamado *temp = (Chamado*)malloc(sizeof(Chamado) * n);
    if (!temp) return;
    size_t i = 0;
    while (!fifo_is_empty(fila->q)) {
        Chamado *ptr = (Chamado*)fifo_dequeue(fila->q);
        if (!ptr) break;
        temp[i++] = *ptr;
        free(ptr);
    }
    for (size_t j = 0; j < i; ++j) {
        cb(&temp[j], ctx);
        enfileirar(fila, temp[j]);
    }
    free(temp);
}
