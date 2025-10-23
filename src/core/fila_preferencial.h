#ifndef HELPDESK_FILA_PREFERENCIAL_H
#define HELPDESK_FILA_PREFERENCIAL_H

#include "atendimento.h"

typedef struct {
    Atendimento *dados;
    int capacidade;
    int tamanho;
} FilaPrioridadeMaxima;

FilaPrioridadeMaxima* criar_fila_prioridade_maxima(int capacidade);

void inserir(FilaPrioridadeMaxima *fila, Atendimento valor);
Atendimento extrair_maximo(FilaPrioridadeMaxima *fila);
void liberar_fila(FilaPrioridadeMaxima *fila);


#endif //HELPDESK_FILA_PREFERENCIAL_H