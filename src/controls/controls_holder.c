#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "controls_holder.h"
#include "teclas.h"
#include "../view/interface.h"
#include "../view/menu.h"
#include "../view/looping.h"
#include "../core/chamado.h"

void handle_controls() {
    enum Tecla tecla = interpretar_tecla();

    switch (tecla) {
        case TECLA_ESC:
        case TECLA_SAIR: {
            Interface *iface = interface_atual();
            if (iface) {
                voltar();
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
            if (max_indice <= 0) break;
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
            if (max_indice <= 0) break;
            opcao_selecionada++;
            if (opcao_selecionada > max_indice) opcao_selecionada = 1;
            break;
        }

        case SETA_ESQ:
        {
            Interface *iface = interface_atual();
            (void)iface;
            voltar();
            opcao_selecionada = 1;
            break;
        }

        case SETA_DIR:
        {
            Interface *iface = interface_atual();
            if (!iface) break;
            for (Opcao *o = iface->primeira_opcao; o; o = o->proxima_opcao) {
                if (o->indice == opcao_selecionada) {
                    if (o->f) o->f();
                    opcao_selecionada = 1;
                    break;
                }
            }
            break;
        }

        default:
        {
            Interface *iface = interface_atual();
            if (iface && iface->tipo == MENSAGEM) {
                // qualquer tecla fecha mensagem
                voltar();
                opcao_selecionada = 1;
            }
            // demais teclas ignoradas; entradas de texto não são mais suportadas aqui
        }
            break;
    }
}
