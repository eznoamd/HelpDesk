#ifndef HELPDESK_BST_TECNICO_H
#define HELPDESK_BST_TECNICO_H

typedef struct Tecnico {
    char nome[100];
    char cpf[14];
    struct Tecnico *esquerda;
    struct Tecnico *direita;
} Tecnico;

Tecnico *criarTecnico(char *nome, char *cpf);
Tecnico *inserirTecnico(Tecnico *raiz, char *nome, char *cpf);
Tecnico *buscarTecnico(Tecnico *raiz, char *nome);
Tecnico *removerTecnico(Tecnico *raiz, char *nome);
void liberarArvoreTecnico(Tecnico *raiz);

#endif //HELPDESK_BST_TECNICO_H