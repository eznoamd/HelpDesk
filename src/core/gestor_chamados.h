#ifndef HELPDESK_GESTOR_CHAMADOS_H
#define HELPDESK_GESTOR_CHAMADOS_H

#include "fila_normal.h"
#include "fila_preferencial.h"
#include "chamado.h"
#include "lista_chamados.h"

void gestor_inicializar();
int gestor_inserir_chamado(Chamado c);

// operações com listas de suspensos e histórico
int gestor_suspender_proximo_normal();
int gestor_suspender_proximo_prioridade();
int gestor_adicionar_historico(Chamado c);

// funções de acesso às filas e listas
FilaNormal* gestor_obter_fila_normal();
FilaPrioridadeMaxima* gestor_obter_fila_prioridade();
ListaChamados* gestor_obter_lista_suspensos();
ListaChamados* gestor_obter_historico();
int gestor_reintegrar_suspenso_por_id(int id);
int gestor_finalizar_suspenso_por_id(int id);

// reavaliar suspensos quando houver entrada de material
int gestor_reavaliar_suspensos_pos_estoque();
void gestor_on_material_entrada();

// cancelamento
int gestor_cancelar_suspenso_por_id(int id);
// remove das filas (prioridade/FIFO) qualquer chamado que case por nome e descricao e adiciona ao historico como Cancelado
int gestor_cancelar_em_filas(const char *nome_pessoa, const char *descricao);
// remove de um tecnico por id e registra como Cancelado
int gestor_cancelar_em_atendimento(const char *nome_tecnico, int id_chamado);

int gestor_cancelar_por_id(const char* nome, int id);


#endif //HELPDESK_GESTOR_CHAMADOS_H
