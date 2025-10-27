#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "controls_holder.h"
#include "teclas.h"
#include "../view/interface.h"
#include "../view/menu.h"
#include "../view/looping.h"

void handle_controls() {
    enum Tecla tecla = interpretar_tecla();

    switch (tecla) {
        case TECLA_ESC:
        case TECLA_SAIR:
        {
            Interface *iface = interface_atual();
            if (iface && iface->tipo == LOGIN) {
                voltar();
                user[0] = '\0';
                pass[0] = '\0';
                opcao_selecionada = 1;
            } else {
                exit(0);
            }
        }
            break;

        case SETA_CIMA:
        {
            Interface *iface = interface_atual();
            if (!iface) break;
            int max_indice = 0;
            for (Opcao *o = iface->primeira_opcao; o; o = o->proxima_opcao) max_indice = o->indice;
            if (max_indice == 0) break;
            opcao_selecionada--;
            if (opcao_selecionada < 1) opcao_selecionada = max_indice;
            break;
        }

        case SETA_BAIXO:
        {
            Interface *iface = interface_atual();
            if (!iface) break;
            int max_indice = 0;
            for (Opcao *o = iface->primeira_opcao; o; o = o->proxima_opcao) max_indice = o->indice;
            if (max_indice == 0) break;
            opcao_selecionada++;
            if (opcao_selecionada > max_indice) opcao_selecionada = 1;
            break;
        }

        case SETA_ESQ:
        {
            Interface *iface = interface_atual();
            voltar();
            break;
        }

        case SETA_DIR:
        {
            Interface *iface = interface_atual();
            if (!iface) break;
            if (iface->tipo == LOGIN) {
                if (login(user, pass)) {
                    abrir_menu_principal();
                    opcao_selecionada = 1;
                    user[0] = '\0';
                    pass[0] = '\0';
                } else {
                    pass[0] = '\0';
                }
            } else {
                for (Opcao *o = iface->primeira_opcao; o; o = o->proxima_opcao) {
                    if (o->indice == opcao_selecionada) {
                        if (o->f) o->f();
                        opcao_selecionada = 1;
                        break;
                    }
                }
            }
            break;
        }

        default:
            {
                Interface *iface = interface_atual();
                if (iface && iface->tipo == LOGIN) {
                    char c = ultima_tecla;
                    if (c == 8 || c == 127) {
                        if (opcao_selecionada == 1) {
                            size_t n = strlen(user);
                            if (n > 0) user[n-1] = '\0';
                        } else if (opcao_selecionada == 2) {
                            size_t n = strlen(pass);
                            if (n > 0) pass[n-1] = '\0';
                        }
                    } else if (c >= 32 && c <= 126) {
                        if (opcao_selecionada == 1) {
                            size_t n = strlen(user);
                            if (n < TAM_LOGIN_STRINGS - 1) { user[n] = c; user[n+1] = '\0'; }
                        } else if (opcao_selecionada == 2) {
                            size_t n = strlen(pass);
                            if (n < TAM_LOGIN_STRINGS - 1) { pass[n] = c; pass[n+1] = '\0'; }
                        }
                    }
                }else {
                    char c = ultima_tecla;
                    if (c >= 48 && c <= 57) {
                        for (Opcao *o = iface->primeira_opcao; o; o = o->proxima_opcao) {
                            if (o->indice == opcao_selecionada) {
                                if (o->f) o->f();
                                opcao_selecionada = 1;
                                break;
                            }
                        }
                    }
                }
            }
            break;
    }
}
