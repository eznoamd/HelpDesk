#include <stdio.h>
#include "./src/view/looping.h"
#include "./src/core/gestor_chamados.h"
#include "./src/core/manager_tecnicos.h"
#include "./src/core/manager_estoque.h"
#include "./src/core/demo.h"

int main() {
    gestor_inicializar();//inicializa os gestor geral de filas 
    manager_tecnicos_inicializar();// inicializa o gestor de tecnicos
    manager_estoque_inicializar();// inicializa o gestor de estoque
    demo_populate();//popula as filas, heaps e bst
    menu();// abre o menu principal
    return 0;
}