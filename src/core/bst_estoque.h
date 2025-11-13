#ifndef HELPDESK_BST_ESTOQUE_H
#define HELPDESK_BST_ESTOQUE_H

typedef struct Produto {
    char nome[100];
    int quantidade;
    struct Produto *esquerda;
    struct Produto *direita;
} Produto;

Produto* criarProduto(char *nome, int quantidade);
Produto* inserirProduto(Produto* raiz, char *nome, int quantidade);
Produto* buscarProduto(Produto* raiz, char *nome);
Produto* removerProduto(Produto* raiz, char *nome);
void liberarArvore(Produto* raiz);

#endif //HELPDESK_BST_ESTOQUE_H