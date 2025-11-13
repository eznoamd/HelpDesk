#ifndef HELPDESK_FILA_NORMAL_H
#define HELPDESK_FILA_NORMAL_H

#include "chamado.h"
#include "../models/fifo.h"

typedef struct {
    FIFO *q;
} FilaNormal;

FilaNormal* criar_fila_normal(int capacidade);
int enfileirar(FilaNormal *fila, Chamado valor);
int desenfileirar(FilaNormal *fila, Chamado *out);
int frente(FilaNormal *fila, Chamado *out);
int fila_vazia(FilaNormal *fila);
void liberar_fila_normal(FilaNormal *fila);

#endif //HELPDESK_FILA_NORMAL_H