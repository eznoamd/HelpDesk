#ifndef HELPDESK_MANAGER_TECNICOS_H
#define HELPDESK_MANAGER_TECNICOS_H

#include "bst_tecnico.h"

void manager_tecnicos_inicializar();
Tecnico* manager_tecnicos_buscar(const char *nome);
Tecnico* manager_tecnicos_inserir(const char *nome, const char *cpf);

// obter raiz (para debug/iteração se necessário)
Tecnico* manager_tecnicos_obter_raiz();

#endif //HELPDESK_MANAGER_TECNICOS_H
