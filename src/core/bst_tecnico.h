#ifndef HELPDESK_BST_TECNICO_H
#define HELPDESK_BST_TECNICO_H

#include "lista_chamados.h"

typedef struct Tecnico {
    char nome[100];
    char cpf[14];
    struct Tecnico *esquerda;
    struct Tecnico *direita;
    ListaChamados *chamados_ativos; // lista duplamente encadeada de chamados atribuídos ao técnico
} Tecnico;

Tecnico *criarTecnico(char *nome, char *cpf);
Tecnico *inserirTecnico(Tecnico *raiz, char *nome, char *cpf);
Tecnico *buscarTecnico(Tecnico *raiz, char *nome);
Tecnico *removerTecnico(Tecnico *raiz, char *nome);
void liberarArvoreTecnico(Tecnico *raiz);

// Operações básicas sobre chamados do técnico
int tecnico_adicionar_chamado(Tecnico *t, Chamado c);
int tecnico_remover_chamado_por_id(Tecnico *t, int id);
int tecnico_remover_e_obter_chamado_por_id(Tecnico *t, int id, Chamado *out);

#endif //HELPDESK_BST_TECNICO_H