    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <time.h>
    #include "menu.h"
    #include "interface.h"
    #include "../core/chamado.h"
    #include "../core/gestor_chamados.h"
    #include "../core/manager_tecnicos.h"
    #include "../core/manager_estoque.h"
    #include "../core/login.h"
    #include "../core/fila_preferencial.h"
    #include "../core/fila_normal.h"
    #include "../core/lista_chamados.h"
    #include "../core/bst_estoque.h"


    static int g_prioridade = PRIORIDADE_1;

    static void cb_sair() { exit(0); }
    static void cb_voltar();

    static void cb_usuario_consultar_status() {
        char nome[AT_NOME_MAX] = {0};
        printf("\nInforme seu nome: "); scanf(" %99[^\n]", nome);
        Interface *m = criar_interface(MENSAGEM, "Status dos Chamados");
        abrir_interface(m);
        int idx = 1; int encontrados = 0;
        adicionar_opcao(m, idx++, "Voltar", cb_voltar);
        // Fila de prioridade
        FilaPrioridadeMaxima *fp = gestor_obter_fila_prioridade();
        if (fp) {
            void cb(const Chamado *c, void *ctx){
                if (strcmp(c->nome_pessoa, nome)==0){
                    char linha[256]; snprintf(linha, sizeof(linha), "(Fila Prioridade) ID %u - %s - P%d", c->id, c->descricao, c->prioridade);
                    adicionar_opcao(m, idx++, linha, NULL); encontrados=1;
                }
            }
            fila_prioridade_iterar(fp, cb, NULL);
        }
        // Fila normal
        FilaNormal *fn = gestor_obter_fila_normal();
        if (fn) {
            void cb2(const Chamado *c, void *ctx){
                if (strcmp(c->nome_pessoa, nome)==0){
                    char linha[256]; snprintf(linha, sizeof(linha), "(Fila FIFO) ID %u - %s - P%d", c->id, c->descricao, c->prioridade);
                    adicionar_opcao(m, idx++, linha, NULL); encontrados=1;
                }
            }
            fila_normal_iterar(fn, cb2, NULL);
        }
        // Suspensos
        ListaChamados *s = gestor_obter_lista_suspensos();
        if (s) {
            for (ChamadoNode *n = s->cabeca; n; n = n->proximo) {
                if (strcmp(n->chamado.nome_pessoa, nome)==0){
                    char linha[256]; snprintf(linha, sizeof(linha), "(Suspenso) ID %u - %s", n->chamado.id, n->chamado.descricao);
                    adicionar_opcao(m, idx++, linha, NULL); encontrados=1;
                }
            }
        }
        // Historico
        ListaChamados *h = gestor_obter_historico();
        if (h) {
            for (ChamadoNode *n = h->cabeca; n; n = n->proximo) {
                if (strcmp(n->chamado.nome_pessoa, nome)==0){
                    char linha[256]; snprintf(linha, sizeof(linha), "(Historico) ID %u - %s - status %d", n->chamado.id, n->chamado.descricao, n->chamado.status);
                    adicionar_opcao(m, idx++, linha, NULL); encontrados=1;
                }
            }
        }
        if (!encontrados) adicionar_opcao(m, idx++, "Nenhum registro encontrado.", NULL);
    }

    static void cb_usuario_cancelar_filas() {
        char nome[AT_NOME_MAX] = {0};
        int id = 0;

        printf("\nSeu nome: "); scanf(" %99[^\n]", nome);
        printf("ID do chamado: "); scanf("%d", &id);

        int ok = gestor_cancelar_por_id(nome, id);
        mostrar_mensagem("Cancelar Chamado", ok ? "Cancelado" : "ID nao encontrado.");
    }

    static void cb_usuario_cancelar_suspenso() {
        // Exige nome e ID e valida correspondencia antes de cancelar
        char nome[AT_NOME_MAX] = {0};
        printf("\nSeu nome: "); scanf(" %99[^\n]", nome);
        int id=0; printf("ID suspenso para cancelar: "); scanf("%d", &id);
        if (id<=0) { mostrar_mensagem("Suspensos", "ID invalido."); return; }
        ListaChamados *s = gestor_obter_lista_suspensos();
        if (!s || !s->cabeca) { mostrar_mensagem("Suspensos", "Nenhum suspenso."); return; }
        ChamadoNode *n = lista_buscar_por_id(s, id);
        if (!n) { mostrar_mensagem("Suspensos", "ID nao encontrado."); return; }
        if (strcmp(n->chamado.nome_pessoa, nome) != 0) { mostrar_mensagem("Suspensos", "ID nao corresponde ao nome."); return; }
        int ok = gestor_cancelar_suspenso_por_id(id);
        mostrar_mensagem("Suspensos", ok?"Cancelamento feito com sucesso":"Falha ao cancelar.");
    }

    static void cb_listar_suspensos() {
        Interface *m = criar_interface(MENSAGEM, "Suspensos");
        abrir_interface(m);
        ListaChamados *s = gestor_obter_lista_suspensos();
        int idx=1;
        adicionar_opcao(m, idx++, "Voltar", cb_voltar);
        if (!s || !s->cabeca){ adicionar_opcao(m, idx++, "(vazio)", NULL); return; }
        for (ChamadoNode *n=s->cabeca;n;n=n->proximo){
            char linha[256]; snprintf(linha,sizeof(linha),"ID %u - %s (%s)", n->chamado.id, n->chamado.nome_pessoa, n->chamado.descricao);
            adicionar_opcao(m, idx++, linha, NULL);
        }
    }

    static void cb_reintegrar_suspenso() {
        int id=0; printf("\nID para reintegrar: "); scanf("%d", &id);
        int ok = gestor_reintegrar_suspenso_por_id(id);
        mostrar_mensagem("Suspensos", ok?"Reintegrado.":"ID invalido.");
    }

    static void cb_cancelar_suspenso() {
        int id=0; printf("\nID para cancelar: "); scanf("%d", &id);
        int ok = gestor_cancelar_suspenso_por_id(id);
        mostrar_mensagem("Suspensos", ok?"Cancelado.":"ID invalido.");
    }

    static void cb_cancelar_meu_chamado() {
        int id=0; printf("\nID do seu chamado para cancelar: "); scanf("%d", &id);
        int ok = gestor_cancelar_em_atendimento(usuario_logado, id);
        mostrar_mensagem("Atendimento", ok?"Cancelado.":"ID nao encontrado.");
    }

    static void cb_ver_historico() {
        Interface *m = criar_interface(MENSAGEM, "Historico");
        abrir_interface(m);
        ListaChamados *h = gestor_obter_historico();
        int idx=1;
        adicionar_opcao(m, idx++, "Voltar", cb_voltar);
        if (!h || !h->cabeca){ adicionar_opcao(m, idx++, "(vazio)", NULL); return; }
        for (ChamadoNode *n=h->cabeca;n;n=n->proximo){
            char linha[256]; snprintf(linha,sizeof(linha),"ID %u - %s - %s (status %d)", n->chamado.id, n->chamado.nome_pessoa, n->chamado.descricao, n->chamado.status);
            adicionar_opcao(m, idx++, linha, NULL);
        }
    }
    static void cb_voltar();

    static void imprimir_tecnicos_rec(Tecnico *r, int *pidx) {
        if (!r) return;
        imprimir_tecnicos_rec(r->esquerda, pidx);
        char linha[256];
        snprintf(linha, sizeof(linha), "%s (CPF: %s)", r->nome, r->cpf);
        Interface *m = interface_atual();
        if (m && m->tipo == MENSAGEM) adicionar_opcao(m, (*pidx)++, linha, NULL);
        imprimir_tecnicos_rec(r->direita, pidx);
    }

    static void cb_listar_tecnicos() {
        Interface *m = criar_interface(MENSAGEM, "Tecnicos Cadastrados");
        abrir_interface(m);
        int idx = 1;
        adicionar_opcao(m, idx++, "Voltar", cb_voltar);
        Tecnico *root = manager_tecnicos_obter_raiz();
        if (!root) {
            adicionar_opcao(m, idx++, "(nenhum)", NULL);
            return;
        }
        imprimir_tecnicos_rec(root, &idx);
    }
    static void cb_voltar() { voltar(); }
    static void cb_menu_usuario() { abrir_menu_usuario(); }
    static void cb_login_tecnico();
    static void cb_menu_tecnico() { abrir_menu_tecnico(); }
    static void cb_menu_estoque() { abrir_menu_estoque(); }
    static void cb_abrir_chamado();
    static void cb_listar_tecnicos();
    static void cb_usuario_consultar_status();
    static void cb_usuario_cancelar_filas();
    static void cb_usuario_cancelar_suspenso();
    static void cb_listar_suspensos();
    static void cb_reintegrar_suspenso();
    static void cb_cancelar_suspenso();
    static void cb_cancelar_meu_chamado();
    static void cb_ver_historico();
    static void cb_ver_filas();

    static void acao_criar_chamado(const char *nome, const char *desc, int prioridade) {
        time_t t = time(NULL); struct tm dt = *localtime(&t);
        Chamado c = chamado_create(desc, prioridade, nome, dt);

        while (1) {
            char opt = 'n';
            printf("\nAdicionar item de estoque ao chamado? (s/n): ");
            scanf(" %c", &opt);
            if (opt == 'n' || opt == 'N') break;

            char item[100] = {0};
            int qtd = 0;
            printf("Nome do item: ");
            scanf(" %99[^\n]", item);
            printf("Quantidade: ");
            scanf("%d", &qtd);
            if (qtd <= 0) {
                printf("Quantidade invalida, ignorando item.\n");
                continue;
            }
            if (!chamado_add_item(&c, item, qtd)) {
                printf("Limite de itens por chamado atingido.\n");
                break;
            }
        }

        gestor_inserir_chamado(c);
    }

    void atualizar_prioridade_from_str(const char *s) {
        if (!s || !*s) { g_prioridade = PRIORIDADE_1; return; }
        int v = atoi(s);
        if (v < PRIORIDADE_1) v = PRIORIDADE_1;
        if (v > PRIORIDADE_5) v = PRIORIDADE_5;
        g_prioridade = v;
    }

    void mostrar_mensagem(const char *titulo, const char *mensagem) {
        Interface *m = criar_interface(MENSAGEM, titulo ? titulo : "Mensagem");
        int idx = 1;
        // Apenas uma opção: pressionar Enter uma vez fecha a mensagem
        adicionar_opcao(m, idx++, mensagem ? mensagem : "OK", cb_voltar);
        abrir_interface(m);
    }

    static void cb_login_tecnico() {
        char u[TAM_LOGIN_STRINGS] = {0};
        char p[TAM_LOGIN_STRINGS] = {0};
        printf("\nUsuario: "); scanf(" %30[^\n]", u);
        printf("Senha  : "); scanf(" %30[^\n]", p);
        if (login(u, p)) {
            // garantir tecnico existente com mesmo nome do usuario logado
            Tecnico *t = manager_tecnicos_buscar(usuario_logado);
            if (!t) {
                char cpf[20] = {0};
                printf("\nPrimeiro acesso deste tecnico. Informe CPF para cadastro: ");
                scanf(" %19[^\n]", cpf);
                manager_tecnicos_inserir(usuario_logado, cpf);
            }
            mostrar_mensagem("Login", "Sucesso.");
            abrir_menu_tecnico();
        } else {
            mostrar_mensagem("Login", "Falhou.");
        }
    }

    static void imprimir_estoque_rec(Produto *r, int *pidx) {
        if (!r) return;
        imprimir_estoque_rec(r->esquerda, pidx);
        char linha[256];
        snprintf(linha, sizeof(linha), "%s - %d", r->nome, r->quantidade);
        Interface *m = interface_atual();
        if (m && m->tipo == MENSAGEM) {
            adicionar_opcao(m, (*pidx)++, linha, NULL);
        } else {
            printf("%s\n", linha);
        }
        imprimir_estoque_rec(r->direita, pidx);
    }

    static void mostrar_lista_estoque() {
        Interface *m = criar_interface(MENSAGEM, "Itens de Estoque");
        abrir_interface(m);
        Produto *raiz = manager_estoque_obter_raiz();
        int idx = 1;
        adicionar_opcao(m, idx++, "Voltar", cb_voltar);
        if (!raiz) { adicionar_opcao(m, idx++, "(vazio)", NULL); return; }
        imprimir_estoque_rec(raiz, &idx);
    }

    static void cb_ver_estoque() { mostrar_lista_estoque(); }

    static void cb_adicionar_material() {
        char item[100] = {0}; int qtd = 0;
        printf("\nItem: "); scanf(" %99[^\n]", item);
        printf("Quantidade: "); scanf("%d", &qtd);
        if (qtd > 0) manager_estoque_inserir(item, qtd);
        mostrar_mensagem("Estoque", "Entrada registrada.");
    }

    static void cb_relatorio_compras() {
        Interface *m = criar_interface(MENSAGEM, "Relatorio de Compras");
        abrir_interface(m);
        ListaChamados *susp = gestor_obter_lista_suspensos();
        int idx = 1;
        adicionar_opcao(m, idx++, "Voltar", cb_voltar);
        if (!susp || !susp->cabeca) { adicionar_opcao(m, idx++, "Nenhum chamado suspenso.", NULL); return; }
        for (ChamadoNode *n = susp->cabeca; n; n = n->proximo) {
            char cab[256];
            snprintf(cab, sizeof(cab), "Chamado %u - %s", n->chamado.id, n->chamado.nome_pessoa);
            adicionar_opcao(m, idx++, cab, NULL);
            for (int k = 0; k < n->chamado.itens_count; ++k) {
                ChamadoItem it = n->chamado.itens[k];
                int tem = manager_estoque_quantidade(it.nome);
                if (tem < it.quantidade) {
                    char linha[256];
                    snprintf(linha, sizeof(linha), "COMPRAR %s (Precisa: %d, Tem: %d)", it.nome, it.quantidade, tem);
                    adicionar_opcao(m, idx++, linha, NULL);
                }
            }
        }
    }

    void abrir_menu_principal() {
        Interface *m = criar_interface(MENU, "Menu Principal");
        adicionar_opcao(m, 1, "Portal do Usuario", cb_menu_usuario);
        adicionar_opcao(m, 2, "Painel do Tecnico", cb_login_tecnico);
        adicionar_opcao(m, 3, "Almoxarifado (Estoque)", cb_menu_estoque);
        adicionar_opcao(m, 4, "Sair", cb_sair);
        abrir_interface(m);
    }

    void abrir_menu_usuario() {
        Interface *m = criar_interface(MENU, "Usuario");
        adicionar_opcao(m, 1, "Abrir Chamado", cb_abrir_chamado);
        adicionar_opcao(m, 2, "Consultar Status do Meu Chamado", cb_usuario_consultar_status);
        adicionar_opcao(m, 3, "Cancelar Chamado (em Filas)", cb_usuario_cancelar_filas);
        adicionar_opcao(m, 4, "Cancelar Chamado Suspenso (por ID)", cb_usuario_cancelar_suspenso);
        adicionar_opcao(m, 5, "Voltar", cb_voltar);
        abrir_interface(m);
    }

    static void cb_abrir_chamado() {
        char nome[AT_NOME_MAX] = {0};
        char problema[AT_DESCRICAO_MAX] = {0};
        int prioridade = PRIORIDADE_1;
        printf("\nUsuario: "); scanf(" %99[^\n]", nome);
        printf("Problema: "); scanf(" %255[^\n]", problema);
        printf("Prioridade (1..5): "); scanf("%d", &prioridade);
        if (prioridade < PRIORIDADE_1) prioridade = PRIORIDADE_1;
        if (prioridade > PRIORIDADE_5) prioridade = PRIORIDADE_5;
        acao_criar_chamado(nome, problema, prioridade);
        mostrar_mensagem("Chamado", "Chamado registrado.");
    }

    static void cb_cadastrar_tecnico() {
        char nome[100] = {0}; char cpf[20] = {0};
        char senha[TAM_LOGIN_STRINGS] = {0};
        printf("\nNome (usuario de login): "); scanf(" %99[^\n]", nome);
        printf("Senha: "); scanf(" %29[^\n]", senha);
        printf("CPF: "); scanf(" %19[^\n]", cpf);
        if (!registrar_usuario(nome, senha)) {
            mostrar_mensagem("Cadastro", "Usuario ja existe ou erro ao salvar.");
            return;
        }
        manager_tecnicos_inserir(nome, cpf);
        mostrar_mensagem("Tecnico", "Tecnico e usuario cadastrados.");
    }

    static void cb_meus_atendimentos() {
        const char *nome = usuario_logado;
        Tecnico *t = manager_tecnicos_buscar(nome);
        Interface *m = criar_interface(MENSAGEM, "Meus Atendimentos");
        abrir_interface(m);
        int idx = 1;
        adicionar_opcao(m, idx++, "Voltar", cb_voltar);
        if (!t || !t->chamados_ativos || !t->chamados_ativos->cabeca) { adicionar_opcao(m, idx++, "(vazio)", NULL); return; }
        for (ChamadoNode *n = t->chamados_ativos->cabeca; n; n = n->proximo) {
            char linha[256];
            snprintf(linha, sizeof(linha), "[%u] %s - %s", n->chamado.id, n->chamado.nome_pessoa, n->chamado.descricao);
            adicionar_opcao(m, idx++, linha, NULL);
        }
    }

    static void cb_ver_filas() {
        Interface *m = criar_interface(MENSAGEM, "Filas de Atendimento");
        abrir_interface(m);
        int idx = 1;
        int encontrados = 0;
        adicionar_opcao(m, idx++, "Voltar", cb_voltar);

        // Fila de prioridade (preferencial)
        FilaPrioridadeMaxima *fp = gestor_obter_fila_prioridade();
        if (fp) {
            void cbp(const Chamado *c, void *ctx) {
                char linha[256];
                snprintf(linha, sizeof(linha), "(Prioridade P%d) ID %u - %s - %s", c->prioridade, c->id, c->nome_pessoa, c->descricao);
                adicionar_opcao(m, idx++, linha, NULL);
                encontrados = 1;
            }
            fila_prioridade_iterar(fp, cbp, NULL);
        }

        // Fila normal (FIFO)
        FilaNormal *fn = gestor_obter_fila_normal();
        if (fn) {
            void cbn(const Chamado *c, void *ctx) {
                char linha[256];
                snprintf(linha, sizeof(linha), "(Normal) ID %u - %s - %s (P%d)", c->id, c->nome_pessoa, c->descricao, c->prioridade);
                adicionar_opcao(m, idx++, linha, NULL);
                encontrados = 1;
            }
            fila_normal_iterar(fn, cbn, NULL);
        }

        if (!encontrados) {
            adicionar_opcao(m, idx++, "(nenhum chamado nas filas)", NULL);
        }
    }

    static int itens_disponiveis(const Chamado *c) {
        for (int k = 0; k < c->itens_count; ++k) {
            int qtd = manager_estoque_quantidade(c->itens[k].nome);
            if (qtd < c->itens[k].quantidade) return 0;
        }
        return 1;
    }

    static void debitar_itens(const Chamado *c) {
        for (int k = 0; k < c->itens_count; ++k) {
            manager_estoque_debitar(c->itens[k].nome, c->itens[k].quantidade);
        }
    }

    static int atender_chamado_para_tecnico(Chamado c) {
        if (itens_disponiveis(&c)) {
            debitar_itens(&c);
            Tecnico *t = manager_tecnicos_buscar(usuario_logado);
            if (t) {
                c.status = CHAMADO_ATRIBUIDO;
                tecnico_adicionar_chamado(t, c);
                mostrar_mensagem("Atendimento", "Chamado assumido.");
                return 1;
            }
            mostrar_mensagem("Erro", "Tecnico nao encontrado.");
            return 0;
        } else {
            ListaChamados *s = gestor_obter_lista_suspensos();
            if (s) { lista_adicionar_chamado(s, c); }
            mostrar_mensagem("Atendimento", "Chamado suspenso (falta material).");
            return 0;
        }
    }

    static void cb_atender_proximo() {
        Chamado c;
        int got = 0;
        FilaPrioridadeMaxima *fp = gestor_obter_fila_prioridade();
        FilaNormal *fn = gestor_obter_fila_normal();
        if (fp) {
            Chamado try = extrair_maximo(fp);
            if (try.descricao[0] != '\0') { c = try; got = 1; }
        }
        if (!got && fn) {
            if (desenfileirar(fn, &c)) got = 1;
        }
        if (!got) { mostrar_mensagem("Atendimento", "Nenhum chamado na fila."); return; }
        // Mostrar detalhes e pedir confirmacao
        printf("\nProximo chamado:\n");
        printf("  Usuario  : %s\n", c.nome_pessoa);
        printf("  Problema : %s\n", c.descricao);
        printf("  Prioridade: %d\n", c.prioridade);
        if (c.itens_count > 0) {
            printf("  Itens necessarios:\n");
            for (int k = 0; k < c.itens_count; ++k) {
                int tem = manager_estoque_quantidade(c.itens[k].nome);
                printf("   - %s: precisa %d, tem %d\n", c.itens[k].nome, c.itens[k].quantidade, tem);
            }
        }
        printf("\nAssumir chamado? (s/n): ");
        char opt = 'n';
        scanf(" %c", &opt);
        if (opt == 'n' || opt == 'N') {
            // reencaminha para as filas seguindo regra do gestor
            gestor_inserir_chamado(c);
            mostrar_mensagem("Atendimento", "Chamado mantido na fila.");
            return;
        }
        atender_chamado_para_tecnico(c);
    }

    static void cb_finalizar_chamado_salvar(int id) {
        Tecnico *t = manager_tecnicos_buscar(usuario_logado);
        if (t && id >= 0) {
            Chamado fin = {0};
            if (tecnico_remover_e_obter_chamado_por_id(t, id, &fin)) {
                fin.status = CHAMADO_FINALIZADO;
                ListaChamados *hist = gestor_obter_historico();
                if (hist) { lista_adicionar_chamado(hist, fin); }
                mostrar_mensagem("Atendimento", "Finalizado.");
            } else {
                mostrar_mensagem("Atendimento", "ID nao encontrado com voce.");
            }
        } else {
            mostrar_mensagem("Erro", "Tecnico ou ID invalido.");
        }
    }

    static void cb_finalizar_chamado() {
        int id = -1;
        printf("\nID para encerrar: "); scanf("%d", &id);
        cb_finalizar_chamado_salvar(id);
    }

    void abrir_menu_tecnico() {
        Interface *m = criar_interface(MENU, "Tecnico");
        adicionar_opcao(m, 1, "Atender Proximo", cb_atender_proximo);
        adicionar_opcao(m, 2, "Meus Atendimentos", cb_meus_atendimentos);
        adicionar_opcao(m, 3, "Ver Filas (Preferencial e Normal)", cb_ver_filas);
        adicionar_opcao(m, 4, "Finalizar Chamado", cb_finalizar_chamado);
        adicionar_opcao(m, 5, "Cancelar Meu Chamado por ID", cb_cancelar_meu_chamado);
        adicionar_opcao(m, 6, "Suspensos - Listar", cb_listar_suspensos);
        adicionar_opcao(m, 7, "Suspensos - Reintegrar por ID", cb_reintegrar_suspenso);
        adicionar_opcao(m, 8, "Suspensos - Cancelar por ID", cb_cancelar_suspenso);
        adicionar_opcao(m, 9, "Historico - Ver recentes", cb_ver_historico);
        adicionar_opcao(m, 10, "Cadastrar Tecnico", cb_cadastrar_tecnico);
        adicionar_opcao(m, 11, "Listar Tecnicos", cb_listar_tecnicos);
        adicionar_opcao(m, 12, "Voltar", cb_voltar);
        abrir_interface(m);
    }

    void abrir_menu_estoque() {
        Interface *m = criar_interface(MENU, "Estoque");
        adicionar_opcao(m, 1, "Ver itens disponiveis", cb_ver_estoque);
        adicionar_opcao(m, 2, "Cadastrar chegada de material", cb_adicionar_material);
        adicionar_opcao(m, 3, "Voltar", cb_voltar);
        abrir_interface(m);
    }

    // login feito inline via scanf; sem interface dedicada
