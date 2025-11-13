#ifndef HELPDESK_CHAMADO_H
#define HELPDESK_CHAMADO_H

#include <time.h>
#include <stddef.h>

#define AT_DESCRICAO_MAX 256
#define AT_NOME_MAX 100

typedef enum {
    SEM_PRIORIDADE = 0,
    PRIORIDADE_1 = 1,
    PRIORIDADE_2 = 2,
    PRIORIDADE_3 = 3,
    PRIORIDADE_4 = 4
} Prioridade;

typedef struct {
    char descricao[AT_DESCRICAO_MAX];
    int prioridade;
    char nome_pessoa[AT_NOME_MAX];
    struct tm data_hora;
} Chamado;

Chamado chamado_create(const char *descricao, int prioridade, const char *nome_pessoa, struct tm data_hora);

#endif