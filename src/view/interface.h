#ifndef HELPDESK_INTERFACE_H
#define HELPDESK_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*Funcao)();

typedef enum {
    FORMULARIO,
    LISTA,
    SELECAO
} TipoMenu;

typedef struct Opcao {
    int indice;
    char nome[64];
    struct Opcao *proxima_opcao;
    Funcao f;
} Opcao;

typedef struct Interface {
    char titulo[64];
    TipoMenu tipo;
    struct Opcao *primeira_opcao;
} Interface;

typedef struct PilhaInterface {
    Interface *interface;
    struct PilhaInterface *proxima;
} PilhaInterface;

void empilhar_interface(Interface *interface);
void desempilhar_interface();
Interface *interface_atual();
void voltar();
Interface *criar_interface(const char *titulo, TipoMenu tipo);
Opcao *criar_opcao(const char *nome, Opcao *anterior, Funcao f);


#endif //HELPDESK_INTERFACE_H