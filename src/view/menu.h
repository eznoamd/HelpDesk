#ifndef HELPDESK_VIEW_MENU_H
#define HELPDESK_VIEW_MENU_H

void abrir_menu_principal();
void abrir_menu_usuario();
void abrir_menu_tecnico();
void abrir_menu_estoque();
void mostrar_mensagem(const char *titulo, const char *mensagem);
void atualizar_prioridade_from_str(const char *s);

#endif //HELPDESK_VIEW_MENU_H
