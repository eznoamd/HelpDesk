#ifndef HELPDESK_MENU_H
#define HELPDESK_MENU_H

typedef struct Interface Interface;

void construir_interfaces();
void abrir_menu_principal();
void abrir_login();
void sair_sistema();

Interface *obter_inicio();

#endif //HELPDESK_MENU_H