#include <stdio.h>
#include <stdlib.h>
#include "looping.h"
#include "menu.h"
#include "interface.h"
#include "../controls/controls_holder.h"

int opcao_selecionada = 1;
char ultima_tecla = 0;
char user[TAM_LOGIN_STRINGS] = {0};
char pass[TAM_LOGIN_STRINGS] = {0};

static void loop_render() {
    Interface *i = interface_atual();
    render_interface(i, opcao_selecionada, user, pass);
}

void menu() {
    abrir_menu_principal();
    while (1) {
        loop_render();
        handle_controls();
    }
}
