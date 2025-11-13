#include "chamado.h"
#include <string.h>

Chamado chamado_create(const char *descricao, int prioridade, const char *nome_pessoa, struct tm data_hora_inicio) {
    Chamado a;
    if (descricao) {
        strncpy(a.descricao, descricao, AT_DESCRICAO_MAX - 1);
        a.descricao[AT_DESCRICAO_MAX - 1] = '\0';
    } else {
        a.descricao[0] = '\0';
    }
    if (prioridade < PRIORIDADE_1) prioridade = PRIORIDADE_1;
    if (prioridade > PRIORIDADE_4) prioridade = PRIORIDADE_4;
    a.prioridade = prioridade;
    if (nome_pessoa) {
        strncpy(a.nome_pessoa, nome_pessoa, AT_NOME_MAX - 1);
        a.nome_pessoa[AT_NOME_MAX - 1] = '\0';
    } else {
        a.nome_pessoa[0] = '\0';
    }
    a.data_hora = data_hora_inicio;
    return a;
}