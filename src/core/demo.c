#include "demo.h"
#include "manager_estoque.h"
#include "manager_tecnicos.h"
#include "gestor_chamados.h"
#include "chamado.h"
#include <time.h>
#include <string.h>

void demo_populate() {
    // add sample products
    manager_estoque_inicializar();
    manager_estoque_inserir("Toner", 2);
    manager_estoque_inserir("Cabo USB", 5);

    // add sample technicians
    manager_tecnicos_inserir("Alice", "11122233344");
    manager_tecnicos_inserir("Bob", "55566677788");

    // add a chamado without items
    time_t t = time(NULL);
    struct tm tm_info = *localtime(&t);
    Chamado c1 = chamado_create("Problema de software", PRIORIDADE_2, "Carlos", tm_info);
    gestor_inserir_chamado(c1);

    // add a chamado that requires Toner (will be atendido until stock runs out)
    Chamado c2 = chamado_create("Substituir toner", PRIORIDADE_1, "Daniela", tm_info);
    c2.itens_count = 1;
    strncpy(c2.itens[0].nome, "Toner", sizeof(c2.itens[0].nome)-1);
    c2.itens[0].nome[sizeof(c2.itens[0].nome)-1] = '\0';
    c2.itens[0].quantidade = 1;
    gestor_inserir_chamado(c2);
}
