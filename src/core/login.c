#include "login.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char usuario_logado[TAM_LOGIN_STRINGS] = "";

typedef struct {
    char usuario[TAM_LOGIN_STRINGS];
    char senha[TAM_LOGIN_STRINGS];
} Usuario;

static Usuario* carregar_usuarios(int *num_usuarios) {
    FILE *fp = fopen(FILE_PATH, "r");
    if (!fp) {
        *num_usuarios = 0;
        return NULL;
    }

    int capacidade = 10;
    *num_usuarios = 0;
    Usuario *lista = malloc(capacidade * sizeof(Usuario));
    if (!lista) {
        fclose(fp);
        return NULL;
    }

    while (fscanf(fp, "%29s %29s", lista[*num_usuarios].usuario, lista[*num_usuarios].senha) == 2) {
        (*num_usuarios)++;
        if (*num_usuarios >= capacidade) {
            capacidade *= 2;
            lista = realloc(lista, capacidade * sizeof(Usuario));
            if (!lista) {
                fclose(fp);
                return NULL;
            }
        }
    }

    fclose(fp);
    return lista;
}

int login(const char *usuario, const char *senha) {
    int num_usuarios;
    Usuario *lista = carregar_usuarios(&num_usuarios);
    if (!lista) {
        return 0;
    }

    int sucesso = 0;
    for (int i = 0; i < num_usuarios; i++) {
        if (strcmp(usuario, lista[i].usuario) == 0 && strcmp(senha, lista[i].senha) == 0) {
            sucesso = 1;
            strncpy(usuario_logado, usuario, TAM_LOGIN_STRINGS - 1);
            usuario_logado[TAM_LOGIN_STRINGS - 1] = '\0';
            break;
        }
    }

    free(lista);
    return sucesso;
}

int get_logged() { return usuario_logado[0] != '\0' ? 1 : 0; }

int registrar_usuario(const char *usuario, const char *senha) {
    if (!usuario || !senha || !*usuario || !*senha) return 0;
    int num_usuarios = 0;
    Usuario *lista = carregar_usuarios(&num_usuarios);
    // se arquivo não existir, carregar_usuarios retorna NULL e num_usuarios=0 (tratado abaixo)
    for (int i = 0; i < num_usuarios; ++i) {
        if (strcmp(usuario, lista[i].usuario) == 0) {
            free(lista);
            return 0; // já existe
        }
    }
    if (lista) free(lista);
    FILE *fp = fopen(FILE_PATH, "a");
    if (!fp) return 0;
    int wrote = fprintf(fp, "%s %s\n", usuario, senha);
    fclose(fp);
    return wrote > 0 ? 1 : 0;
}