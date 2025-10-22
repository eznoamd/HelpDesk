#ifndef HELPDESK_TECLAS_H
#define HELPDESK_TECLAS_H

enum Tecla {
    TECLA_NORMAL = 0,
    SETA_CIMA,
    SETA_BAIXO,
    SETA_ESQ,
    SETA_DIR,
    TECLA_ESC,
    TECLA_SAIR
};

char getch_cross();
enum Tecla interpretar_tecla();

#endif //HELPDESK_TECLAS_H