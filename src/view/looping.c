#include <stdio.h>
#include <stdlib.h>
#include "widgets.h"
#include "menu.h"
#include "interface.h"
#include "looping.h"
#include "../controls/controls_holder.h"

int opcao_selecionada = 1;
char ultima_tecla = 0;
char user[TAM_LOGIN_STRINGS] = "";
char pass[TAM_LOGIN_STRINGS] = "";

void menu() {
    construir_interfaces();
    empilhar_interface(obter_inicio());

    while (1) {
        system("cls");
        Interface *iface = interface_atual();
        if (!iface) {
            iface = obter_inicio();
        }
        menu_box(iface);
        handle_controls();
    }
}