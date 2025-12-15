#ifndef HELPDESK_VIEW_INTERFACE_H
#define HELPDESK_VIEW_INTERFACE_H

#include <stddef.h>

typedef enum {
    MENU = 0,
    MENSAGEM = 1
} InterfaceTipo;

typedef struct Opcao {
    int indice;
    char *nome;
    void (*f)(void);
    struct Opcao *proxima_opcao;
    char *valor;
    int valor_tamanho;
} Opcao;

typedef struct Interface {
    InterfaceTipo tipo;
    char titulo[128];
    Opcao *primeira_opcao;
    struct Interface *anterior;
} Interface;

Interface *interface_atual();
void abrir_interface(Interface *i);
void voltar();

// helpers para construir UI
Interface *criar_interface(InterfaceTipo tipo, const char *titulo);
void liberar_interface(Interface *i);
Opcao *adicionar_opcao(Interface *i, int indice, const char *nome, void (*f)(void));
void render_interface(const Interface *i, int opcao_selecionada, const char *user, const char *pass);

#endif //HELPDESK_VIEW_INTERFACE_H
