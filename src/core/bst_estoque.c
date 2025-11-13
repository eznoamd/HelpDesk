#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst_estoque.h"

Produto* criarProduto(char *nome, int quantidade) {
    Produto* p = (Produto*) malloc(sizeof(Produto));
    strcpy(p->nome, nome);
    p->quantidade = quantidade;
    p->esquerda = NULL;
    p->direita = NULL;
    return p;
}

Produto* inserirProduto(Produto* raiz, char *nome, int quantidade) {
    if (raiz == NULL) {
        return criarProduto(nome, quantidade);
    }
    if (strcmp(nome, raiz->nome) < 0) {
        raiz->esquerda = inserirProduto(raiz->esquerda, nome, quantidade);
    } else {
        raiz->direita = inserirProduto(raiz->direita, nome, quantidade);
    }
    return raiz;
}

Produto* buscarProduto(Produto* raiz, char *nome) {
    if (raiz == NULL || strcmp(nome, raiz->nome) == 0) {
        return raiz;
    }
    if (strcmp(nome, raiz->nome) < 0) {
        return buscarProduto(raiz->esquerda, nome);
    } else {
        return buscarProduto(raiz->direita, nome);
    }
}

Produto* removerProduto(Produto* raiz, char *nome) {
    if (raiz == NULL) {
        return raiz;
    }
    if (strcmp(nome, raiz->nome) < 0) {
        raiz->esquerda = removerProduto(raiz->esquerda, nome);
    } else if (strcmp(nome, raiz->nome) > 0) {
        raiz->direita = removerProduto(raiz->direita, nome);
    } else {
        if (raiz->esquerda == NULL) {
            Produto* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            Produto* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        Produto* temp = buscarProduto(raiz->direita, nome);
        strcpy(raiz->nome, temp->nome);
        raiz->quantidade = temp->quantidade;
        raiz->direita = removerProduto(raiz->direita, nome);
    }
    return raiz;
}

void liberarArvore(Produto* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}