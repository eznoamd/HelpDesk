#ifndef HELPDESK_MENU_H
#define HELPDESK_MENU_H

typedef struct Interface Interface;

void construir_interfaces();
void abrir_menu_cliente();
void abrir_menu_atendente();
void entrar_menu_atendente();
void voltar_inicio();
void sair_sistema();

Interface *obter_inicio();

#endif //HELPDESK_MENU_H