#ifndef HELPDESK_LOGIN_H
#define HELPDESK_LOGIN_H

#define TAM_LOGIN_STRINGS 30
#define FILE_PATH "../src/data/usuarios.txt"

extern char usuario_logado[TAM_LOGIN_STRINGS];

int login(const char *usuario, const char *senha);
int get_logged();

#endif //HELPDESK_LOGIN_H