#include "manager_estoque.h"
#include "gestor_chamados.h"
#include <string.h>
#include <stdlib.h>

static Produto *estoque_root = NULL;

void manager_estoque_inicializar() {
    estoque_root = NULL;
}

int manager_estoque_inserir(const char *nome, int quantidade) {
    if (!nome || quantidade <= 0) return 0;

    Produto *p = buscarProduto(estoque_root, nome);

    if (p) {
        p->quantidade += quantidade;
        gestor_on_material_entrada();
        return 1;
    }

    estoque_root = inserirProduto(estoque_root, nome, quantidade);

    gestor_on_material_entrada();
    return 1;
}

int manager_estoque_debitar(const char *nome, int quantidade) {
    if (!nome || quantidade <= 0) return 0;

    Produto *p = buscarProduto(estoque_root, nome);
    if (!p) return 0;
    if (p->quantidade < quantidade) return 0;

    p->quantidade -= quantidade;

    if (p->quantidade == 0)
        estoque_root = removerProduto(estoque_root, nome);

    return 1;
}

int manager_estoque_quantidade(const char *nome) {
    if (!nome) return 0;

    Produto *p = buscarProduto(estoque_root, nome);
    return p ? p->quantidade : 0;
}

Produto* manager_estoque_obter_raiz() {
    return estoque_root;
}
