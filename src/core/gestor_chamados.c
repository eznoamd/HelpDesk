#include "gestor_chamados.h"
#include "lista_chamados.h"
#include "manager_estoque.h"
#include "manager_tecnicos.h"
#include <stdlib.h>
#include <string.h>

static FilaNormal *fila_normal_global = NULL;
static FilaPrioridadeMaxima *fila_prioridade_global = NULL;
static ListaChamados *lista_suspensos = NULL;
static ListaChamados *lista_historico = NULL;
static unsigned int g_proximo_chamado_id = 1; // gerador global de IDs de chamados

void gestor_inicializar() {
    if (!fila_normal_global) fila_normal_global = criar_fila_normal(32);
    if (!fila_prioridade_global) fila_prioridade_global = criar_fila_prioridade_maxima(32);
    if (!lista_suspensos) lista_suspensos = criar_lista_chamados();
    if (!lista_historico) lista_historico = criar_lista_chamados();
}

int gestor_cancelar_em_atendimento(const char *nome_tecnico, int id_chamado) {
    if (!nome_tecnico || id_chamado <= 0) return 0;
    Tecnico *t = manager_tecnicos_buscar(nome_tecnico);
    if (!t) return 0;
    Chamado c = {0};
    if (!tecnico_remover_e_obter_chamado_por_id(t, id_chamado, &c)) return 0;
    c.status = CHAMADO_CANCELADO;
    return gestor_adicionar_historico(c);
}

int gestor_inserir_chamado(Chamado c) {
    if (!fila_normal_global || !fila_prioridade_global || !lista_suspensos || !lista_historico) gestor_inicializar();
    // garantir que todo chamado tenha um ID unico
    if (c.id == 0) {
        c.id = g_proximo_chamado_id++;
    }
    // regra: prioridade alta vai para fila de prioridade (>= 3), senao FIFO
    if (c.prioridade >= PRIORIDADE_3) {
        inserir(fila_prioridade_global, c);
        return 1;
    } else {
        return enfileirar(fila_normal_global, c);
    }
}

// remove o próximo chamado da fila normal e coloca em suspensos
int gestor_suspender_proximo_normal() {
    if (!fila_normal_global) return 0;
    Chamado c;
    if (!desenfileirar(fila_normal_global, &c)) return 0;
    return lista_adicionar_chamado(lista_suspensos, c) != NULL;
}

// remove o próximo (máxima prioridade) da fila de prioridade e coloca em suspensos
int gestor_suspender_proximo_prioridade() {
    if (!fila_prioridade_global) return 0;
    Chamado c = extrair_maximo(fila_prioridade_global);
    // extrair_maximo deve retornar um Chamado "vazio" quando sem itens (implementacao da fila)
    if (c.descricao[0] == '\0' && c.nome_pessoa[0] == '\0') return 0;
    return lista_adicionar_chamado(lista_suspensos, c) != NULL;
}

int gestor_adicionar_historico(Chamado c) {
    if (!lista_historico) lista_historico = criar_lista_chamados();
    // por seguranca, garante que o historico nao receba chamados sem ID
    if (c.id == 0) {
        c.id = g_proximo_chamado_id++;
    }
    return lista_adicionar_chamado(lista_historico, c) != NULL;
}

FilaNormal* gestor_obter_fila_normal() { return fila_normal_global; }
FilaPrioridadeMaxima* gestor_obter_fila_prioridade() { return fila_prioridade_global; }
ListaChamados* gestor_obter_lista_suspensos() { return lista_suspensos; }
ListaChamados* gestor_obter_historico() { return lista_historico; }

int gestor_reintegrar_suspenso_por_id(int id) {
    if (!lista_suspensos) return 0;
    ChamadoNode *n = lista_buscar_por_id(lista_suspensos, id);
    if (!n) return 0;
    Chamado c = n->chamado;
    if (!lista_remover_no(lista_suspensos, n)) return 0;
    // reinsere preservando prioridade original
    return gestor_inserir_chamado(c);
}

int gestor_finalizar_suspenso_por_id(int id) {
    if (!lista_suspensos) return 0;
    ChamadoNode *n = lista_buscar_por_id(lista_suspensos, id);
    if (!n) return 0;
    Chamado c = n->chamado;
    if (!lista_remover_no(lista_suspensos, n)) return 0;
    return gestor_adicionar_historico(c);
}

static int itens_disponiveis(const Chamado *c) {
    for (int k = 0; k < c->itens_count; ++k) {
        int qtd = manager_estoque_quantidade(c->itens[k].nome);
        if (qtd < c->itens[k].quantidade) return 0;
    }
    return 1;
}

int gestor_reavaliar_suspensos_pos_estoque() {
    if (!lista_suspensos) return 0;
    int movidos = 0;
    ChamadoNode *cur = lista_suspensos->cabeca;
    while (cur) {
        ChamadoNode *proximo = cur->proximo; // salvar antes de possivel remoção
        if (itens_disponiveis(&cur->chamado)) {
            Chamado c = cur->chamado;
            // remove nó atual da lista de suspensos
            c->prioridade = PRIORIDADE_5;
            if (lista_remover_no(lista_suspensos, cur)) {
                // preserva prioridade original ao re-colocar nas filas
                gestor_inserir_chamado(c);
                movidos++;
            }
        }
        cur = proximo;
    }
    return movidos;
}

void gestor_on_material_entrada() {
    gestor_reavaliar_suspensos_pos_estoque();
}

int gestor_cancelar_suspenso_por_id(int id) {
    if (!lista_suspensos) return 0;
    ChamadoNode *n = lista_buscar_por_id(lista_suspensos, id);
    if (!n) return 0;
    Chamado c = n->chamado;
    c.status = CHAMADO_CANCELADO;
    if (!lista_remover_no(lista_suspensos, n)) return 0;
    return gestor_adicionar_historico(c);
}

static int chamado_match(const Chamado *c, const char *nome_pessoa, const char *descricao) {
    if (!c) return 0;
    int ok_nome = (!nome_pessoa || *nome_pessoa == '\0') || strcmp(c->nome_pessoa, nome_pessoa) == 0;
    int ok_desc = (!descricao || *descricao == '\0') || strcmp(c->descricao, descricao) == 0;
    return ok_nome && ok_desc;
}

int gestor_cancelar_em_filas(const char *nome_pessoa, const char *descricao) {
    int cancelados = 0;
    // Prioridade: extrair todos, não re-inserir os que casarem
    if (fila_prioridade_global) {
        Chamado *buffer = NULL; size_t cap = 0, n = 0;
        while (1) {
            Chamado x = extrair_maximo(fila_prioridade_global);
            if (x.descricao[0] == '\0' && x.nome_pessoa[0] == '\0') break;
            if (chamado_match(&x, nome_pessoa, descricao)) {
                x.status = CHAMADO_CANCELADO;
                gestor_adicionar_historico(x);
                cancelados++;
            } else {
                if (n == cap) { cap = cap ? cap*2 : 8; buffer = (Chamado*)realloc(buffer, sizeof(Chamado)*cap); }
                buffer[n++] = x;
            }
        }
        for (size_t i = 0; i < n; ++i) inserir(fila_prioridade_global, buffer[i]);
        free(buffer);
    }
    // FIFO: remover todos, não re-enfileirar os que casarem
    if (fila_normal_global) {
        Chamado *buffer = NULL; size_t cap = 0, n = 0;
        Chamado temp;
        while (desenfileirar(fila_normal_global, &temp)) {
            if (chamado_match(&temp, nome_pessoa, descricao)) {
                temp.status = CHAMADO_CANCELADO;
                gestor_adicionar_historico(temp);
                cancelados++;
            } else {
                if (n == cap) { cap = cap ? cap*2 : 8; buffer = (Chamado*)realloc(buffer, sizeof(Chamado)*cap); }
                buffer[n++] = temp;
            }
        }
        for (size_t i = 0; i < n; ++i) enfileirar(fila_normal_global, buffer[i]);
        free(buffer);
    }
    return cancelados;
}

int gestor_cancelar_por_id(const char* nome_pessoa, int id)
{
    int removido = 0;

    /*
     * 1. Procurar na FILA DE PRIORIDADE (heap)
     */
    if (fila_prioridade_global) {
        Chamado *buffer = NULL;
        size_t cap = 0, n = 0;

        while (1) {
            Chamado x = extrair_maximo(fila_prioridade_global);
            if (x.descricao[0] == '\0' && x.nome_pessoa[0] == '\0')
                break;

            if (x.id == id && strcmp(x.nome_pessoa, nome_pessoa) == 0) {
                x.status = CHAMADO_CANCELADO;
                gestor_adicionar_historico(x);
                removido = 1;
            } else {
                if (n == cap) {
                    cap = cap ? cap * 2 : 8;
                    buffer = realloc(buffer, sizeof(Chamado) * cap);
                }
                buffer[n++] = x;
            }
        }

        for (size_t i = 0; i < n; ++i)
            inserir(fila_prioridade_global, buffer[i]);

        free(buffer);
    }

    /*
     * 2. Procurar na FILA NORMAL (FIFO)
     */
    if (!removido && fila_normal_global) {
        Chamado *buffer = NULL;
        size_t cap = 0, n = 0;
        Chamado temp;

        while (desenfileirar(fila_normal_global, &temp)) {
            if (temp.id == id && strcmp(temp.nome_pessoa, nome_pessoa) == 0) {
                temp.status = CHAMADO_CANCELADO;
                gestor_adicionar_historico(temp);
                removido = 1;
            } else {
                if (n == cap) {
                    cap = cap ? cap * 2 : 8;
                    buffer = realloc(buffer, sizeof(Chamado) * cap);
                }
                buffer[n++] = temp;
            }
        }

        for (size_t i = 0; i < n; ++i)
            enfileirar(fila_normal_global, buffer[i]);

        free(buffer);
    }

    /*
     * 3. Procurar na LISTA DE SUSPENSOS
     */
    if (!removido && lista_suspensos) {
        ChamadoNode *n = lista_buscar_por_id(lista_suspensos, id);
        if (n && strcmp(n->chamado.nome_pessoa, nome_pessoa) == 0) {
            Chamado c = n->chamado;
            c.status = CHAMADO_CANCELADO;
            lista_remover_no(lista_suspensos, n);
            gestor_adicionar_historico(c);
            removido = 1;
        }
    }

    return removido;
}
