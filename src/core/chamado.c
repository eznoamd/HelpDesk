#include "chamado.h"
#include <string.h>
#include <stdio.h>

Chamado chamado_create(const char *descricao, Prioridade prioridade, const char *nome_pessoa, struct tm data_hora_inicio) {
    Chamado a = {0};

    if (descricao) {
        strncpy(a.descricao, descricao, AT_DESCRICAO_MAX - 1);
    }

    if (prioridade < SEM_PRIORIDADE) prioridade = SEM_PRIORIDADE;
    if (prioridade > PRIORIDADE_5) prioridade = PRIORIDADE_5;
    a.prioridade = prioridade;

    if (nome_pessoa) {
        strncpy(a.nome_pessoa, nome_pessoa, AT_NOME_MAX - 1);
    }

    a.data_hora = data_hora_inicio;
    a.status = CHAMADO_ABERTO;
    a.itens_count = 0;

    return a;
}

int chamado_add_item(Chamado *c, const char *nome, int quantidade) {
    if (c->itens_count >= CHAMADO_MAX_ITENS) return 0;

    ChamadoItem *it = &c->itens[c->itens_count++];
    strncpy(it->nome, nome, 99);
    it->nome[99] = '\0';
    it->quantidade = quantidade;

    return 1;
}
