#ifndef HELPDESK_CONTROLS_HOLDER_H
#define HELPDESK_CONTROLS_HOLDER_H
#include "../core/login.h"

extern char ultima_tecla;
extern int opcao_selecionada;
extern char user[TAM_LOGIN_STRINGS];
extern char pass[TAM_LOGIN_STRINGS];

void handle_controls();

#endif //HELPDESK_CONTROLS_HOLDER_H