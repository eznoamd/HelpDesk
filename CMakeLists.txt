cmake_minimum_required(VERSION 4.0)
project(HelpDesk C)

set(CMAKE_C_STANDARD 11)

add_executable(HelpDesk main.c
        src/view/interface.c
        src/view/interface.h
        src/view/menu.c
        src/core/login.c
        src/core/login.h
        src/view/widgets.c
        src/view/looping.c
        src/view/menu.h
        src/view/widgets.h
        src/view/looping.h
        src/controls/teclas.c
        src/controls/teclas.h
        src/controls/controls_holder.h
        src/controls/controls_holder.c
        src/view/acoes.c
        src/view/acoes.h
        src/core/fila_normal.c
        src/core/fila_normal.h
        src/core/fila_preferencial.c
        src/core/fila_preferencial.h
        src/core/chamado.c
        src/core/chamado.h
        src/core/bst_estoque.c
        src/core/bst_estoque.h
        src/core/bst_tecnico.c
        src/core/bst_tecnico.h
        src/view/color.h
        src/models/fifo.c
        src/models/fifo.h
        src/models/heap.c
        src/models/heap.h
)
