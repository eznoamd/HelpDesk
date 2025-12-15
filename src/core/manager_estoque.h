#ifndef HELPDESK_MANAGER_ESTOQUE_H
#define HELPDESK_MANAGER_ESTOQUE_H

#include "bst_estoque.h"

void manager_estoque_inicializar();
int manager_estoque_inserir(const char *nome, int quantidade);
int manager_estoque_debitar(const char *nome, int quantidade);
int manager_estoque_quantidade(const char *nome);
Produto* manager_estoque_obter_raiz();

#endif
