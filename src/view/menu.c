#include "interface.h"
#include "menu.h"
#include "../controls/controls_holder.h"

Interface *inicio;
Interface *menu_cliente;
Interface *menu_atendente;
Interface *form_login;

void construir_interfaces() {
    inicio = criar_interface("Selecione o tipo de usuario", SELECAO);
    Opcao *i1 = criar_opcao("Cliente", NULL, 0, abrir_menu_principal);
    Opcao *i2 = criar_opcao("Atendente", i1, 0, abrir_login);
    Opcao *i3 = criar_opcao("Sair", i2, 0, sair_sistema);
    inicio->primeira_opcao = i1;

    form_login = criar_interface("Login - Atendente", LOGIN);
    Opcao *l1 = criar_opcao("Usuario", NULL, 0, NULL);
    Opcao *l2 = criar_opcao("Senha", l1, 0, NULL);
    form_login->primeira_opcao = l1;

    menu_cliente = criar_interface("Menu Principal - Cliente", SELECAO);
    Opcao *c1 = criar_opcao("Iniciar Atendimento", NULL, 0, NULL);
    Opcao *c2 = criar_opcao("Atender pedidos", c1, 1, NULL);
    Opcao *c3 = criar_opcao("Ver meus atendimentos", c2, 0, NULL);
    Opcao *c4 = criar_opcao("Sair", c3, 0,sair_sistema);
    menu_cliente->primeira_opcao = c1;
}

void abrir_menu_principal() { empilhar_interface(menu_cliente); }
void abrir_login() { empilhar_interface(form_login); }
void voltar_inicio() { while (interface_atual() != inicio) desempilhar_interface(); }
void sair_sistema() { exit(0); }

Interface *obter_inicio() { return inicio; }