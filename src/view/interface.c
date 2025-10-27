#include "interface.h"

static PilhaInterface *pilha = NULL;

void empilhar_interface(Interface *interface) {
    PilhaInterface *novo = malloc(sizeof(PilhaInterface));
    if (!novo) {
        printf("Erro ao alocar memória para pilha.\n");
        exit(1);
    }
    novo->interface = interface;
    novo->proxima = pilha;
    pilha = novo;
}

void desempilhar_interface() {
    if (pilha) {
        PilhaInterface *topo = pilha;
        pilha = pilha->proxima;
        free(topo);
    }
}

Interface *interface_atual() {
    return pilha ? pilha->interface : NULL;
}

void voltar() {
    desempilhar_interface();
}

Interface *criar_interface(const char *titulo, TipoMenu tipo) {
    Interface *nova = malloc(sizeof(Interface));
    if (!nova) {
        printf("Erro ao alocar memória para interface.\n");
        exit(1);
    }
    strcpy(nova->titulo, titulo);
    nova->tipo = tipo;
    nova->primeira_opcao = NULL;
    return nova;
}

Opcao *criar_opcao(const char *nome, Opcao *anterior, int adm, Funcao f) {
    Opcao *nova = malloc(sizeof(Opcao));
    if (!nova) {
        printf("Erro ao alocar memória para opção.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->f = f;
    nova->proxima_opcao = NULL;
    nova->indice = anterior ? anterior->indice + 1 : 1;

    if (anterior) anterior->proxima_opcao = nova;

    return nova;
}
