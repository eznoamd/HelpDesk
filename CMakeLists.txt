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
)
