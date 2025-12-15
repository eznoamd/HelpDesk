#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst_estoque.h"

// cria produto
Produto* criarProduto(const char *nome, int quantidade) {
    Produto* p = (Produto*) malloc(sizeof(Produto));
    strncpy(p->nome, nome, 99);
    p->nome[99] = '\0';

    p->quantidade = quantidade;
    p->esquerda = NULL;
    p->direita = NULL;
    return p;
}

Produto* inserirProduto(Produto* raiz, const char *nome, int quantidade) {
    if (raiz == NULL) {
        return criarProduto(nome, quantidade);
    }

    int cmp = strcmp(nome, raiz->nome);

    if (cmp < 0) {
        raiz->esquerda = inserirProduto(raiz->esquerda, nome, quantidade);
    } else if (cmp > 0) {
        raiz->direita = inserirProduto(raiz->direita, nome, quantidade);
    } else {
        // nomes iguais → soma estoque
        raiz->quantidade += quantidade;
    }

    return raiz;
}

Produto* buscarProduto(Produto* raiz, const char *nome) {
    if (raiz == NULL) return NULL;

    int cmp = strcmp(nome, raiz->nome);

    if (cmp == 0) return raiz;
    if (cmp < 0) return buscarProduto(raiz->esquerda, nome);
    return buscarProduto(raiz->direita, nome);
}

// encontra o menor elemento da árvore
static Produto* minimo(Produto *n) {
    while (n && n->esquerda)
        n = n->esquerda;
    return n;
}

Produto* removerProduto(Produto* raiz, const char *nome) {
    if (raiz == NULL) return NULL;

    int cmp = strcmp(nome, raiz->nome);

    if (cmp < 0) {
        raiz->esquerda = removerProduto(raiz->esquerda, nome);
    }
    else if (cmp > 0) {
        raiz->direita = removerProduto(raiz->direita, nome);
    }
    else {
        // nó encontrado
        if (raiz->esquerda == NULL) {
            Produto* temp = raiz->direita;
            free(raiz);
            return temp;
        }
        else if (raiz->direita == NULL) {
            Produto* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        // sucessor: menor da subárvore direita
        Produto* temp = minimo(raiz->direita);

        strcpy(raiz->nome, temp->nome);
        raiz->quantidade = temp->quantidade;

        raiz->direita = removerProduto(raiz->direita, temp->nome);
    }

    return raiz;
}

void liberarArvore(Produto* raiz) {
    if (!raiz) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}
