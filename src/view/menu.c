#include "interface.h"
#include "menu.h"
#include "../controls/controls_holder.h"

Interface *inicio;
Interface *menu_cliente;
Interface *menu_atendente;
Interface *login_atendente;

void construir_interfaces() {
    inicio = criar_interface("Selecione o tipo de usuario", SELECAO);
    Opcao *i1 = criar_opcao("Cliente", NULL, abrir_menu_cliente);
    Opcao *i2 = criar_opcao("Atendente", i1, abrir_menu_atendente);
    Opcao *i3 = criar_opcao("Sair", i2, sair_sistema);
    inicio->primeira_opcao = i1;

    menu_cliente = criar_interface("Menu Principal - Cliente", SELECAO);
    Opcao *c1 = criar_opcao("Iniciar Atendimento", NULL, NULL);
    Opcao *c2 = criar_opcao("Ver Meus Atendimentos", c1, NULL);
    Opcao *c3 = criar_opcao("Voltar", c2, voltar_inicio);
    menu_cliente->primeira_opcao = c1;

    login_atendente = criar_interface("Login - Atendente", FORMULARIO);
    Opcao *l1 = criar_opcao("Usuario", NULL, NULL);
    Opcao *l2 = criar_opcao("Senha", l1, NULL);
    login_atendente->primeira_opcao = l1;
}

void abrir_menu_cliente() { empilhar_interface(menu_cliente); }
void abrir_menu_atendente() { empilhar_interface(login_atendente); }
void voltar_inicio() { while (interface_atual() != inicio) desempilhar_interface(); }
void sair_sistema() { exit(0); }

Interface *obter_inicio() { return inicio; }