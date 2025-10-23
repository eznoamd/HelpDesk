#include "fila_preferencial.h"
#include <stdio.h>
#include <stdlib.h>

static void trocar(Atendimento *a, Atendimento *b) {
    Atendimento temp = *a;
    *a = *b;
    *b = temp;
}

static int data_antes(const struct tm *a, const struct tm *b) {
    if (a->tm_year != b->tm_year) return a->tm_year < b->tm_year;
    if (a->tm_mon  != b->tm_mon)  return a->tm_mon  < b->tm_mon;
    if (a->tm_mday != b->tm_mday) return a->tm_mday < b->tm_mday;
    if (a->tm_hour != b->tm_hour) return a->tm_hour < b->tm_hour;
    if (a->tm_min  != b->tm_min)  return a->tm_min  < b->tm_min;
    if (a->tm_sec  != b->tm_sec)  return a->tm_sec  < b->tm_sec;
    return 0;
}

static int maior_que(const Atendimento *a, const Atendimento *b) {
    if (a->prioridade != b->prioridade) {
        return a->prioridade > b->prioridade;
    }
    return data_antes(&a->data_hora, &b->data_hora);
}

static int garantir_capacidade(FilaPrioridadeMaxima *fila, int capacidade_minima) {
    if (fila->capacidade >= capacidade_minima) {
        return 1;
    }
    int nova_capacidade = fila->capacidade > 0 ? fila->capacidade : 1;
    while (nova_capacidade < capacidade_minima) {
        nova_capacidade *= 2;
    }
    Atendimento *novos_dados = (Atendimento*)realloc(fila->dados, nova_capacidade * sizeof(Atendimento));
    if (novos_dados == NULL) {
        printf("Falha ao realocar memoria\n");
        return 0;
    }
    fila->dados = novos_dados;
    fila->capacidade = nova_capacidade;
    return 1;
}

static void arrumar_acima(FilaPrioridadeMaxima *fila, int indice) {
    int i = indice;
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (maior_que(&fila->dados[pai], &fila->dados[i])) {
            break;
        }
        trocar(&fila->dados[pai], &fila->dados[i]);
        i = pai;
    }
}

static void arrumar_abaixo(FilaPrioridadeMaxima *fila, int indice) {
    int i = indice;
    while (1) {
        int maior = i;
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;
        if (esq < fila->tamanho && maior_que(&fila->dados[esq], &fila->dados[maior])) {
            maior = esq;
        }
        if (dir < fila->tamanho && maior_que(&fila->dados[dir], &fila->dados[maior])) {
            maior = dir;
        }
        if (maior == i) {
            break;
        }
        trocar(&fila->dados[i], &fila->dados[maior]);
        i = maior;
    }
}

FilaPrioridadeMaxima* criar_fila_prioridade_maxima(int capacidade) {
    FilaPrioridadeMaxima *fila = (FilaPrioridadeMaxima*)malloc(sizeof(FilaPrioridadeMaxima));
    fila->dados = (Atendimento*)malloc(capacidade * sizeof(Atendimento));
    fila->capacidade = capacidade;
    fila->tamanho = 0;
    return fila;
}

void inserir(FilaPrioridadeMaxima *fila, Atendimento valor) {
    if (fila->tamanho == fila->capacidade) {
        if (!garantir_capacidade(fila, fila->tamanho + 1)) {
            printf("Overflow do heap\n");
            return;
        }
    }

    fila->tamanho++;
    int indice = fila->tamanho - 1;
    fila->dados[indice] = valor;
    arrumar_acima(fila, indice);
}

Atendimento extrair_maximo(FilaPrioridadeMaxima *fila) {
    Atendimento vazio;
    vazio.descricao[0] = '\0';
    vazio.prioridade = 0;
    vazio.nome_pessoa[0] = '\0';
    vazio.data_hora = (struct tm){0};
    if (fila->tamanho == 0) {
        printf("Underflow do heap\n");
        return vazio;
    }

    Atendimento maximo = fila->dados[0];
    fila->dados[0] = fila->dados[fila->tamanho - 1];
    fila->tamanho--;
    arrumar_abaixo(fila, 0);

    return maximo;
}

void liberar_fila(FilaPrioridadeMaxima *fila) {
    free(fila->dados);
    free(fila);
}
