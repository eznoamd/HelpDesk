#ifndef HELPDESK_LISTA_CHAMADOS_H
#define HELPDESK_LISTA_CHAMADOS_H

#include "chamado.h"
#include <stddef.h>


typedef struct ChamadoNode {
    Chamado chamado;
    struct ChamadoNode *anterior; // ponteiro para o nó anterior
    struct ChamadoNode *proximo;  // ponteiro para o próximo nó
} ChamadoNode;

typedef struct ListaChamados {
    ChamadoNode *cabeca; // primeiro nó
    ChamadoNode *cauda;  // último nó
    size_t tamanho;      // número de elementos
} ListaChamados;

ListaChamados* criar_lista_chamados();
void liberar_lista_chamados(ListaChamados *l);
ChamadoNode* lista_adicionar_chamado(ListaChamados *l, Chamado c);
int lista_remover_no(ListaChamados *l, ChamadoNode *n);
ChamadoNode* lista_buscar_por_id(ListaChamados *l, int id);

#endif //HELPDESK_LISTA_CHAMADOS_H
