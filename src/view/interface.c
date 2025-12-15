#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "interface.h"
#include "../controls/controls_holder.h"
#include "color.h"
#include "../core/login.h"

static Interface *g_atual = NULL;

Interface *interface_atual() { return g_atual; }

void abrir_interface(Interface *i) {
    if (!i) return;
    i->anterior = g_atual;
    g_atual = i;
}

void voltar() {
    if (!g_atual) return;
    Interface *ant = g_atual->anterior;
    // liberar interface atual
    Interface *tmp = g_atual;
    g_atual = ant;
    liberar_interface(tmp);
}

Interface *criar_interface(InterfaceTipo tipo, const char *titulo) {
    Interface *i = (Interface*)calloc(1, sizeof(Interface));
    if (!i) return NULL;
    i->tipo = tipo;
    i->primeira_opcao = NULL;
    i->anterior = NULL;
    if (titulo) {
        snprintf(i->titulo, sizeof(i->titulo), "%s", titulo);
    } else {
        i->titulo[0] = '\0';
    }
    return i;
}

void liberar_interface(Interface *i) {
    if (!i) return;
    Opcao *o = i->primeira_opcao;
    while (o) {
        Opcao *n = o->proxima_opcao;
        if (o->valor) free(o->valor);
        free(o);
        o = n;
    }
    free(i);
}

Opcao *adicionar_opcao(Interface *i, int indice, const char *nome, void (*f)(void)) {
    if (!i) return NULL;
    Opcao *o = (Opcao*)calloc(1, sizeof(Opcao));
    if (!o) return NULL;
    o->indice = indice;
    if (nome) {
        size_t len = strlen(nome);
        o->nome = (char*)malloc(len + 1);
        if (o->nome) { memcpy(o->nome, nome, len + 1); }
    }
    o->f = f;
    o->proxima_opcao = NULL;
    o->valor = NULL;
    o->valor_tamanho = 0;
    if (!i->primeira_opcao) {
        i->primeira_opcao = o;
    } else {
        Opcao *p = i->primeira_opcao;
        while (p->proxima_opcao) p = p->proxima_opcao;
        p->proxima_opcao = o;
    }
    return o;
}

static void limpar_tela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void render_interface(const Interface *i, int sel, const char *user_in, const char *pass_in) {
    if (!i) return;
    limpar_tela();
    const char *usuario = (get_logged() ? usuario_logado : "Anonimo");
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char dateonly[16] = {0};
    strftime(dateonly, sizeof(dateonly), "%d/%m/%Y", tm_info);

    printf("\n\n");
    {
        int largura_total = 84;
        int len_usuario = (int)strlen(usuario);
        int len_data = (int)strlen(dateonly);
        int espacos = largura_total - len_usuario - len_data - 2;
        if (espacos < 1) espacos = 1;
        printf("\033[38;5;226m %s%*s%s %s", usuario, espacos, "", dateonly, RESET);
    }

    {
        int largura_total = 82;
        const char *title = "HELP DESK";
        int len_title = (int)strlen(title);
        int total_tracos = largura_total - 2 - len_title;
        if (total_tracos < 0) total_tracos = 0;
        int esq = total_tracos / 2;
        int dir = total_tracos - esq;
        printf("%s .%.*s%s%.*s. %s", YEL1, esq, "----------------------------------------------------------------------------", title, dir, "----------------------------------------------------------------------------", RESET);
    }

    printf(YEL1 " |.------------------------------------------------------------------------------.| " RESET);
    printf(YEL1 " ||                                                                              || " RESET);
    printf(DEG1 " ||  .::   .: .,::::::   :::  ::::::::::.:::::::-.  .,:::::: .::::::.  :::  ..   || " RESET);
    printf(DEG2 " ||  ;;;   ;;,;;;;''''   ;;;   `;;;```.;;;;;,   `';,;;;;'''';;;`    `  ;;; .;;,. || " RESET);
    printf(DEG3 " || ,[[[,,,[[[ [[cccc    [[[    `]]nnn]]' `[[     [[ [[cccc '[==/[[[[, [[[[[/'   || " RESET);
    printf(DEG4 " || \"$$$\"\"\"$$$ $$\"\"\"    $$'     $$$\"\"     $$,    $$ $$\"\"\"\"   '''    $_$$$$,      || " RESET);
    printf(DEG5 " ||  888   \"88o888oo,__ o88oo,.__888o      888_,o8P' 888oo,__88b    dP\"888\"88o,  || " RESET);
    printf(DEG6 " ||  MMM    YMM\"\"\"\"YUMMM\"\"\"\"YUMMMYMMMb     MMMMP\"`   \"\"\"\"YUMMM\"YMmMY\"  MMM \"MMP\" || " RESET);
    printf(RED1 " ||                                                                              || " RESET);
    printf(RED1 " |'------------------------------------------------------------------------------'| " RESET);
    printf(RED1 " '--------------------------------------------------------------------------------' " RESET);
    // Após a moldura superior, seguimos direto para o conteúdo (seleção ou formulário)
    printf("\n");

    int count = 0;
    const int left_pad = 4; // aproxima a coluna onde o conteúdo iniciava dentro da grade
    for (const Opcao *o = i->primeira_opcao; o; o = o->proxima_opcao) {
        count++;
        int color = (o->indice == sel) ? 214 : 124;
        for (int sp = 0; sp < left_pad; ++sp) putchar(' ');
        printf("\033[38;5;%dm[%d] %s\033[0m\n", color, o->indice, o->nome ? o->nome : "");
    }
    if (count == 0) {
        for (int sp = 0; sp < left_pad; ++sp) putchar(' ');
        printf("(Sem opcoes)\n");
    }
}
