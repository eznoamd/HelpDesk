#include <stdio.h>
#include <time.h>
#include <string.h>
#include "menu.h"
#include "interface.h"
#include "../core/login.h"
#include "looping.h"
#include "../controls/controls_holder.h"

const char* print_usuario_com_data(const char *usuario);
void print_cabecalho(const char *title, int layer, int red);
void print_opcao(int num, const char *texto, int selecionada);
void print_opcoes(Interface *interface, int selecionada);
static void print_login_linha(const char *label, const char *valor, int selecionado, int is_password);

void menu_box(Interface *interface) {
    printf("\n\n");
    const char *usuario = (get_logged() ? usuario_logado : "Anonimo");

    print_usuario_com_data(usuario);
    print_cabecalho("HELP DESK", 0, 0);
    printf("\033[38;5;226m |.------------------------------------------------------------------------------.| \033[0m\n");
    printf("\033[38;5;226m ||                                                                              || \033[0m\n");
    printf("\033[38;5;220m ||  .::   .: .,::::::   :::  ::::::::::.:::::::-.  .,:::::: .::::::.  :::  ..   || \033[0m\n");
    printf("\033[38;5;214m ||  ;;;   ;;,;;;;''''   ;;;   `;;;```.;;;;;,   `';,;;;;'''';;;`    `  ;;; .;;,. || \033[0m\n");
    printf("\033[38;5;208m || ,[[[,,,[[[ [[cccc    [[[    `]]nnn]]' `[[     [[ [[cccc '[==/[[[[, [[[[[/'   || \033[0m\n");
    printf("\033[38;5;202m || \"$$$\"\"\"$$$ $$\"\"\"    $$'     $$$\"\"     $$,    $$ $$\"\"\"\"   '''    $_$$$$,      || \033[0m\n");
    printf("\033[38;5;196m ||  888   \"88o888oo,__ o88oo,.__888o      888_,o8P' 888oo,__88b    dP\"888\"88o,  || \033[0m\n");
    printf("\033[38;5;160m ||  MMM    YMM\"\"\"\"YUMMM\"\"\"\"YUMMMYMMMb     MMMMP\"`   \"\"\"\"YUMMM\"YMmMY\"  MMM \"MMP\" || \033[0m\n");
    printf("\033[38;5;124m ||                                                                              || \033[0m\n");
    printf("\033[38;5;124m |'------------------------------------------------------------------------------'| \033[0m\n");
    printf("\033[38;5;124m | -> Aperte o numero desejado para selecionar a opcao                            | \033[0m\n");
    printf("\033[38;5;124m | -> Use as setas cima e baixo para escolher opcao                               | \033[0m\n");
    printf("\033[38;5;124m | -> Use a seta direita para selecinar                                           | \033[0m\n");
    printf("\033[38;5;124m | -> Aperte 'q' para sair                                                        | \033[0m\n");
    print_cabecalho(interface->titulo, 1, 1);
    printf("\033[38;5;124m ||                                                                              || \033[0m\n");


    switch(interface->tipo) {
        case LOGIN:
            print_login_linha("Usuario:", user, opcao_selecionada == 1, 0);
            print_login_linha("Senha:  ", pass, opcao_selecionada == 2, 1);
            break;
        case SELECAO:
            print_opcoes(interface, opcao_selecionada);
            break;
        case LISTA:
            break;
    }

    printf("\033[38;5;124m |'------------------------------------------------------------------------------'| \033[0m\n");
    printf("\033[38;5;124m '--------------------------------------------------------------------------------' \033[0m\n");

    printf("\033[0m");
}

static void print_login_linha(const char *label, const char *valor, int selecionado, int is_password) {
    int largura_total = 82;
    int color_label = selecionado ? 214 : 124;
    int color_value = selecionado ? 214 : 124;

    char buffer[TAM_LOGIN_STRINGS];
    if (is_password) {
        size_t n = strlen(valor);
        size_t m = n < sizeof(buffer)-1 ? n : sizeof(buffer)-1;
        for (size_t i = 0; i < m; ++i) buffer[i] = '*';
        buffer[m] = '\0';
    } else {
        strncpy(buffer, valor, sizeof(buffer));
        buffer[sizeof(buffer)-1] = '\0';
    }

    char linha[256];
    snprintf(linha, sizeof(linha), "%s %s", label, buffer);

    int len_linha = (int)strlen(linha);
    int espacos = largura_total - 6 - len_linha;
    if (espacos < 0) espacos = 0;

    printf("\033[38;5;124m || \033[0m");
    printf("\033[38;5;%dm%s ", color_label, label);
    printf("\033[38;5;%dm%s\033[0m", color_value, buffer);
    for (int i = 0; i < espacos; i++) putchar(' ');
    printf("\033[38;5;124m ||\033[0m\n");
}

const char* print_usuario_com_data(const char *usuario) {
    static char linha[128];
    char dateonly[16];

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(dateonly, sizeof(dateonly), "%d/%m/%Y", tm_info);

    int largura_total = 84;
    int len_usuario = (int)strlen(usuario);
    int len_data = (int)strlen(dateonly);

    int espacos = largura_total - len_usuario - len_data - 2;
    if (espacos < 1) espacos = 1;

    snprintf(linha, sizeof(linha), "%s%*s%s", usuario, espacos, "", dateonly);
    printf("\033[38;5;226m %s \033[0m\n", linha);
}

void print_cabecalho(const char *title, int layer, int red) {
    int largura_total = 82;
    int len_title = (int)strlen(title);

    const char *inicio = (layer == 0) ? "." : "|.";
    const char *fim = (layer == 0) ? "." : ".|";

    int len_extremos = (int)strlen(inicio) + (int)strlen(fim);
    int total_tracos = largura_total - len_extremos - len_title;

    if (total_tracos < 0) total_tracos = 0;

    int tracos_esquerda = total_tracos / 2;
    int tracos_direita = total_tracos - tracos_esquerda;

    int color = red ? 124 : 226;

    printf("\033[38;5;%dm %s%.*s%s%.*s%s \033[0m\n",
           color,
           inicio,
           tracos_esquerda, "----------------------------------------------------------------------------",
           title,
           tracos_direita, "----------------------------------------------------------------------------",
           fim);
}

void print_opcao(int num, const char *texto, int selecionada) {
    int largura_total = 82;
    int len_num = snprintf(NULL, 0, "%d", num);
    int len_texto = (int)strlen(texto);

    int pontos = largura_total - 6 - 3 - len_num - len_texto;
    if (pontos < 0) pontos = 0;

    printf("\033[38;5;124m || \033[0m");

    int color = selecionada ? 214 : 124;
    printf("\033[38;5;%dm[%d] %s", color, num, texto);

    for (int i = 0; i < pontos; i++) putchar(' ');

    printf("\033[38;5;124m ||\033[0m\n");
}

void print_opcoes(Interface * interface, int selecionada) {
    Opcao * opcao = interface->primeira_opcao;
    while (opcao != NULL) {
        int s = (selecionada == opcao->indice);
        print_opcao(opcao->indice, opcao->nome, s);
        opcao = opcao->proxima_opcao;
    }
}