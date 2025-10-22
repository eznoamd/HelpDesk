#include <stdio.h>
#include "teclas.h"
#include "controls_holder.h"

#ifdef _WIN32
    #include <conio.h>
    void limpar_buffer_teclado() {
        while (_kbhit()) _getch();
    }
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>

    void limpar_buffer_teclado() {
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

        while (getchar() != EOF);

        fcntl(STDIN_FILENO, F_SETFL, flags);
    }
#endif

char getch_cross() {
    limpar_buffer_teclado();
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

enum Tecla interpretar_tecla() {
#ifdef _WIN32
    int ch = _getch();

    if (ch == 27)
        return TECLA_ESC;

    if (ch == 13)
        return SETA_DIR;

    if (ch == 0 || ch == 224) {
        ch = _getch();
        switch (ch) {
            case 72: return SETA_CIMA;
            case 80: return SETA_BAIXO;
            case 75: return SETA_ESQ;
            case 77: return SETA_DIR;
        }
    }

    if (ch == 'q' || ch == 'Q') return TECLA_SAIR;
    ultima_tecla = (char)ch;
    return TECLA_NORMAL;

#else
    char c = getch_cross();

    if (c == '\n' || c == '\r') return SETA_DIR;

    if (c == 27) {
        char n1 = getch_cross();
        if (n1 == '[') {
            char n2 = getch_cross();
            switch (n2) {
                case 'A': return SETA_CIMA;
                case 'B': return SETA_BAIXO;
                case 'C': return SETA_DIR;
                case 'D': return SETA_ESQ;
            }
        } else {
            return TECLA_ESC;
        }
    }

    if (c == 'q' || c == 'Q') return TECLA_SAIR;
    ultima_tecla = c;
    return TECLA_NORMAL;
#endif
}
