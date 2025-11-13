#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst_tecnico.h"

Tecnico *criarTecnico(char *nome, char *cpf) {
    Tecnico *t = (Tecnico *) malloc(sizeof(Tecnico));
    strcpy(t->nome, nome);
    strcpy(t->cpf, cpf);
    t->esquerda = NULL;
    t->direita = NULL;
    return t;
}

Tecnico *inserirTecnico(Tecnico *raiz, char *nome, char *cpf) {
    if (raiz == NULL) {
        return criarTecnico(nome, cpf);
    }
    if (strcmp(nome, raiz->nome) < 0) {
        raiz->esquerda = inserirTecnico(raiz->esquerda, nome, cpf);
    } else {
        raiz->direita = inserirTecnico(raiz->direita, nome, cpf);
    }
    return raiz;
}

Tecnico *buscarTecnico(Tecnico *raiz, char *nome) {
    if (raiz == NULL || strcmp(nome, raiz->nome) == 0) {
        return raiz;
    }
    if (strcmp(nome, raiz->nome) < 0) {
        return buscarTecnico(raiz->esquerda, nome);
    } else {
        return buscarTecnico(raiz->direita, nome);
    }
}

Tecnico *removerTecnico(Tecnico *raiz, char *nome) {
    if (raiz == NULL) {
        return raiz;
    }
    if (strcmp(nome, raiz->nome) < 0) {
        raiz->esquerda = removerTecnico(raiz->esquerda, nome);
    } else if (strcmp(nome, raiz->nome) > 0) {
        raiz->direita = removerTecnico(raiz->direita, nome);
    } else {
        if (raiz->esquerda == NULL) {
            Tecnico *temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            Tecnico *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        Tecnico *temp = buscarTecnico(raiz->direita, nome);
        strcpy(raiz->nome, temp->nome);
        strcpy(raiz->cpf, temp->cpf);
        raiz->direita = removerTecnico(raiz->direita, nome);
    }
    return raiz;
}

void liberarArvoreTecnico(Tecnico *raiz) {
    if (raiz != NULL) {
        liberarArvoreTecnico(raiz->esquerda);
        liberarArvoreTecnico(raiz->direita);
        free(raiz);
    }
}
