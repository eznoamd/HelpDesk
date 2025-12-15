#include "lista_chamados.h"
#include <stdlib.h>
#include <string.h>

ListaChamados* criar_lista_chamados() {
    ListaChamados *l = (ListaChamados*)malloc(sizeof(ListaChamados));
    if (!l) return NULL;
    l->cabeca = l->cauda = NULL;
    l->tamanho = 0;
    return l;
}

void liberar_lista_chamados(ListaChamados *l) {
    if (!l) return;
    ChamadoNode *cur = l->cabeca;
    while (cur) {
        ChamadoNode *n = cur;
        cur = cur->proximo;
        free(n);
    }
    free(l);
}

ChamadoNode* lista_adicionar_chamado(ListaChamados *l, Chamado c) {
    if (!l) return NULL;
    ChamadoNode *n = (ChamadoNode*)malloc(sizeof(ChamadoNode));
    if (!n) return NULL;
    n->chamado = c;
    n->anterior = l->cauda;
    n->proximo = NULL;
    if (l->cauda) l->cauda->proximo = n;
    l->cauda = n;
    if (!l->cabeca) l->cabeca = n;
    l->tamanho++;
    return n;
}

int lista_remover_no(ListaChamados *l, ChamadoNode *n) {
    if (!l || !n) return 0;
    if (n->anterior) n->anterior->proximo = n->proximo; else l->cabeca = n->proximo;
    if (n->proximo) n->proximo->anterior = n->anterior; else l->cauda = n->anterior;
    free(n);
    if (l->tamanho > 0) l->tamanho--;
    return 1;
}

ChamadoNode* lista_buscar_por_id(ListaChamados *l, int id) {
    if (!l) return NULL;
    ChamadoNode *cur = l->cabeca;
    while (cur) {
        if ((int)cur->chamado.id == id) return cur;
        cur = cur->proximo;
    }
    return NULL;
}
