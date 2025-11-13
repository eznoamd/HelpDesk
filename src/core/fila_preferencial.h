#ifndef HELPDESK_FILA_PREFERENCIAL_H
#define HELPDESK_FILA_PREFERENCIAL_H

#include "chamado.h"
#include "../models/heap.h"

typedef struct {
    Heap *h;
} FilaPrioridadeMaxima;

FilaPrioridadeMaxima* criar_fila_prioridade_maxima(int capacidade);

void inserir(FilaPrioridadeMaxima *fila, Chamado valor);
Chamado extrair_maximo(FilaPrioridadeMaxima *fila);
void liberar_fila(FilaPrioridadeMaxima *fila);


#endif //HELPDESK_FILA_PREFERENCIAL_H