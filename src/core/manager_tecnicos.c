#include "manager_tecnicos.h"
#include <stdlib.h>
#include <string.h>

static Tecnico *raiz_tecnicos = NULL;

void manager_tecnicos_inicializar() {
    // raiz mantida vazia até inserções; se quiser, pré-popular aqui
}

Tecnico* manager_tecnicos_buscar(const char *nome) {
    if (!raiz_tecnicos) return NULL;
    return buscarTecnico(raiz_tecnicos, (char*)nome);
}

Tecnico* manager_tecnicos_inserir(const char *nome, const char *cpf) {
    if (!raiz_tecnicos) {
        raiz_tecnicos = criarTecnico((char*)nome, (char*)cpf);
        return raiz_tecnicos;
    }
    raiz_tecnicos = inserirTecnico(raiz_tecnicos, (char*)nome, (char*)cpf);
    return buscarTecnico(raiz_tecnicos, (char*)nome);
}

Tecnico* manager_tecnicos_obter_raiz() { return raiz_tecnicos; }
