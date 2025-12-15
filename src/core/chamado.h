#ifndef HELPDESK_CHAMADO_H
#define HELPDESK_CHAMADO_H

#include <time.h>

#define AT_DESCRICAO_MAX 256
#define AT_NOME_MAX 100
#define CHAMADO_MAX_ITENS 10

typedef enum {
    SEM_PRIORIDADE = 0,
    PRIORIDADE_1 = 1,
    PRIORIDADE_2 = 2,
    PRIORIDADE_3 = 3,
    PRIORIDADE_4 = 4,
    PRIORIDADE_5 = 5
} Prioridade;

typedef struct {
    char nome[100];
    int quantidade;
} ChamadoItem;

typedef enum {
    CHAMADO_ABERTO = 0,
    CHAMADO_SUSPENSO = 1,
    CHAMADO_ATRIBUIDO = 2,
    CHAMADO_FINALIZADO = 3,
    CHAMADO_CANCELADO = 4
} ChamadoStatus;

typedef struct {
    unsigned int id;
    char descricao[AT_DESCRICAO_MAX];
    Prioridade prioridade;
    char nome_pessoa[AT_NOME_MAX];
    struct tm data_hora;
    ChamadoStatus status;
    char tecnico_atribuido[AT_NOME_MAX];
    ChamadoItem itens[CHAMADO_MAX_ITENS];
    int itens_count;
} Chamado;


Chamado chamado_create(const char *descricao, Prioridade prioridade, const char *nome_pessoa, struct tm data_hora);
int chamado_add_item(Chamado *c, const char *nome, int quantidade);

#endif
