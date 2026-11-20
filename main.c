#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipamentos.h"
#include "solicitacoes.h"
#include "operacoes.h"
#include "historico.h"
#include "arquivos.h"

typedef struct
{
    Equipamento *equipamentos; // Lista de equipamentos.
    Fila fila; // Fila de solicitacoes pendentes.
    Operacao *operacoes; // Pilha de operacoes recentes (topo).
    Movimentacao *historico; // Lista do historico de movimentacoes.
    int proximo_protocolo; // Proximo numero de protocolo a ser gerado.
    char data[TAM_DATA_OP]; // Data de hoje, digitada ao iniciar o programa.
} Sistema;

void ler_texto(const char *mensagem, char *dest, int tam)
{
    printf("%s", mensagem);
    if (fgets(dest, tam, stdin) == NULL) // Se a leitura falhar, deixa o texto vazio.
    {
        dest[0] = '\0';
        return;
    }
    dest[strcspn(dest, "\n")] = '\0'; // Troca o \n do fgets por fim de texto.
}

int ler_inteiro(const char *mensagem)
{
    char linha[32];
    int valor;

    printf("%s", mensagem);
    while (fgets(linha, sizeof(linha), stdin) == NULL || sscanf(linha, "%d", &valor) != 1) // Repete ate ler um numero.
    {
        printf("Entrada invalida. Digite um numero: ");
    }
    return valor;
}

void registrar_operacao(Sistema *s, const char *descricao)
{
    empilhar_operacao(&s->operacoes, descricao, s->data); // Empilha a operacao realizada com sucesso.
}

void registrar_movimentacao(Sistema *s, int tipo, int id, int protocolo, int matricula, int entrada, const char *nome)
{
    Movimentacao m;
    memset(&m, 0, sizeof(Movimentacao)); // Zera todos os campos.
    m.tipo = tipo;
    m.id_equipamento = id;
    m.protocolo = protocolo;
    m.matricula = matricula;
    m.entrada_manutencao = entrada;
    strcpy(m.nome_responsavel, nome);
    inserir_movimentacao(&s->historico, m); // Registra no historico.
}

void mostrar_menu(void)
{
    printf("\n===== Controle da lampisteria =====\n");
    printf("1  - Cadastrar equipamento\n");
    printf("2  - Listar equipamentos\n");
    printf("3  - Buscar equipamento\n");
    printf("4  - Registrar solicitacao\n");
    printf("5  - Listar solicitacoes pendentes\n");
    printf("6  - Atender proxima solicitacao\n");
    printf("7  - Registrar devolucao\n");
    printf("8  - Alterar manutencao\n");
    printf("9  - Cancelar solicitacao\n");
    printf("10 - Mostrar estado do sistema\n");
    printf("11 - Salvar dados\n");
    printf("12 - Carregar dados\n");
    printf("0  - Sair\n");
}

void cadastrar_equipamento(Sistema *s)
{
    Equipamento e;
    char texto[TAM_DESC_OP];

    memset(&e, 0, sizeof(Equipamento)); // Zera todos os campos.
    e.id = ler_inteiro("ID: ");

    if (buscar_equipamento(s->equipamentos, e.id) != NULL) // Recusa ID repetido.
    {
        printf("Erro: ja existe equipamento com identificador %d.\n", e.id);
        return;
    }

    ler_texto("Descricao: ", e.descricao, TAM_DESCRICAO);
    ler_texto("Categoria: ", e.categoria, TAM_CATEGORIA);
    ler_texto("Patrimonio: ", e.patrimonio, TAM_PATRIMONIO);
    e.estado = DISPONIVEL; // Todo equipamento novo comeca disponivel.

    if (!inserir_equipamento(&s->equipamentos, e))
    {
        printf("Erro: nao foi possivel cadastrar o equipamento.\n");
        return;
    }

    printf("Equipamento %d cadastrado.\n", e.id);
    snprintf(texto, sizeof(texto), "Cadastro do equipamento %d", e.id);
    registrar_operacao(s, texto);
}

void buscar_equipamento_menu(Sistema *s)
{
    int opcao = ler_inteiro("(1) buscar por ID (2) listar por categoria: ");

    if (opcao == 1)
    {
        int id = ler_inteiro("ID: ");
        Equipamento *e = buscar_equipamento(s->equipamentos, id);

        if (e == NULL) // Identificador inexistente.
        {
            printf("Erro: equipamento %d nao encontrado.\n", id);
            return;
        }
        printf("ID | Descricao | Categoria | Patrimonio | Estado | Responsavel\n");
        imprimir_equipamento(e);
    }
    else if (opcao == 2)
    {
        char categoria[TAM_CATEGORIA];
        ler_texto("Categoria: ", categoria, TAM_CATEGORIA);
        listar_por_categoria(s->equipamentos, categoria);
    }
    else
    {
        printf("Erro: opcao invalida.\n");
    }
}

void registrar_solicitacao(Sistema *s)
{
    Solicitacao sol;
    char texto[TAM_DESC_OP];

    memset(&sol, 0, sizeof(Solicitacao)); // Zera todos os campos.
    ler_texto("Nome: ", sol.nome_solicitante, TAM_NOME);
    sol.matricula = ler_inteiro("Matricula: ");
    ler_texto("Categoria: ", sol.categoria, TAM_CATEGORIA);
    ler_texto("Local de trabalho: ", sol.local, TAM_LOCAL);
    ler_texto("Data (dd/mm/aaaa): ", sol.data, TAM_DATA);

    if (solicitante_possui_categoria(s->equipamentos, sol.matricula, sol.categoria)) // Regra: um equipamento por categoria.
    {
        printf("Erro: %s (%d) ja possui um %s emprestado.\n",
               sol.nome_solicitante, sol.matricula, sol.categoria);
        return;
    }

    sol.protocolo = s->proximo_protocolo; // Protocolo sequencial gerado pelo sistema.
    s->proximo_protocolo++;
    enfileirar(&s->fila, sol); // Entra no final da fila.

    printf("Solicitacao registrada. Protocolo: %d. Posicao na fila: %d.\n",
           sol.protocolo, s->fila.tamanho);
    snprintf(texto, sizeof(texto), "Solicitacao registrada: protocolo %d (%s)",
             sol.protocolo, sol.nome_solicitante);
    registrar_operacao(s, texto);
}

void atender_proxima(Sistema *s)
{
    char texto[TAM_DESC_OP];

    if (s->fila.inicio == NULL) // Fila vazia, nao ha o que atender.
    {
        printf("Erro: fila de solicitacoes vazia.\n");
        return;
    }

    Solicitacao sol = *s->fila.inicio; // Copia o primeiro da fila sem remover.
    Equipamento *equip = emprestar_equipamento(s->equipamentos, sol.categoria,
                                               sol.nome_solicitante, sol.matricula);

    if (equip != NULL) // Achou equipamento disponivel: remove da fila e registra.
    {
        Solicitacao removida;
        desenfileirar(&s->fila, &removida);

        printf("Protocolo %d (%s): equipamento %d emprestado.\n",
               sol.protocolo, sol.nome_solicitante, equip->id);
        snprintf(texto, sizeof(texto), "Emprestimo do equipamento %d a %s (protocolo %d)",
                 equip->id, sol.nome_solicitante, sol.protocolo);
        registrar_operacao(s, texto);
        registrar_movimentacao(s, MOV_EMPRESTIMO, equip->id, sol.protocolo,
                               sol.matricula, 0, sol.nome_solicitante);
        return;
    }

    printf("Protocolo %d (%s): nao ha %s disponivel.\n",
           sol.protocolo, sol.nome_solicitante, sol.categoria);

    int opcao = 0;
    while (opcao < 1 || opcao > 3) // Repete ate o usuario escolher uma opcao valida.
    {
        opcao = ler_inteiro("(1) manter no inicio (2) recolocar no final (3) cancelar : ");
    }

    if (opcao == 1) // Manter: a solicitacao nunca saiu do inicio da fila.
    {
        printf("Protocolo %d mantido no inicio da fila.\n", sol.protocolo);
    }
    else if (opcao == 2) // Recolocar: remove do inicio e insere no final.
    {
        Solicitacao removida;
        desenfileirar(&s->fila, &removida);
        enfileirar(&s->fila, removida);

        printf("Protocolo %d recolocado no final da fila.\n", sol.protocolo);
        snprintf(texto, sizeof(texto), "Protocolo %d recolocado no final da fila", sol.protocolo);
        registrar_operacao(s, texto);
    }
    else // Cancelar: remove da fila e registra no historico.
    {
        Solicitacao removida;
        desenfileirar(&s->fila, &removida);

        printf("Protocolo %d cancelado.\n", removida.protocolo);
        snprintf(texto, sizeof(texto), "Cancelamento do protocolo %d (%s)",
                 removida.protocolo, removida.nome_solicitante);
        registrar_operacao(s, texto);
        registrar_movimentacao(s, MOV_CANCELAMENTO, 0, removida.protocolo,
                               removida.matricula, 0, removida.nome_solicitante);
    }
}

void registrar_devolucao(Sistema *s)
{
    char texto[TAM_DESC_OP];
    Equipamento devolvido;
    int id = ler_inteiro("ID do equipamento: ");
    int resultado = devolver_equipamento(s->equipamentos, id, &devolvido);

    if (resultado == 0) // Equipamento inexistente.
    {
        printf("Erro: equipamento %d nao encontrado.\n", id);
    }
    else if (resultado == -1) // Equipamento nao estava emprestado.
    {
        printf("Erro: equipamento %d nao esta emprestado.\n", id);
    }
    else
    {
        printf("Devolucao registrada. Equipamento %d disponivel.\n", id);
        snprintf(texto, sizeof(texto), "Devolucao do equipamento %d (%s)",
                 id, devolvido.nome_responsavel);
        registrar_operacao(s, texto);
        registrar_movimentacao(s, MOV_DEVOLUCAO, id, 0, devolvido.matricula_responsavel,
                               0, devolvido.nome_responsavel);
    }
}

void alterar_manutencao_menu(Sistema *s)
{
    char texto[TAM_DESC_OP];
    int id = ler_inteiro("ID do equipamento: ");
    int resultado = alterar_manutencao(s->equipamentos, id);

    if (resultado == 0) // Equipamento inexistente.
    {
        printf("Erro: equipamento %d nao encontrado.\n", id);
    }
    else if (resultado == -1) // Emprestado nao pode entrar em manutencao.
    {
        printf("Erro: equipamento %d esta emprestado e nao pode entrar em manutencao.\n", id);
    }
    else if (resultado == 1) // Entrou em manutencao.
    {
        printf("Equipamento %d enviado para manutencao.\n", id);
        snprintf(texto, sizeof(texto), "Equipamento %d enviado para manutencao", id);
        registrar_operacao(s, texto);
        registrar_movimentacao(s, MOV_MANUTENCAO, id, 0, 0, 1, "");
    }
    else // Saiu da manutencao.
    {
        printf("Equipamento %d retornou da manutencao e esta disponivel.\n", id);
        snprintf(texto, sizeof(texto), "Equipamento %d retornou da manutencao", id);
        registrar_operacao(s, texto);
        registrar_movimentacao(s, MOV_MANUTENCAO, id, 0, 0, 0, "");
    }
}

void cancelar_solicitacao_menu(Sistema *s)
{
    char texto[TAM_DESC_OP];
    Solicitacao cancelada;
    int protocolo = ler_inteiro("Protocolo: ");

    if (cancelar_solicitacao(&s->fila, protocolo, &cancelada))
    {
        printf("Protocolo %d cancelado.\n", protocolo);
        snprintf(texto, sizeof(texto), "Cancelamento do protocolo %d (%s)",
                 protocolo, cancelada.nome_solicitante);
        registrar_operacao(s, texto);
        registrar_movimentacao(s, MOV_CANCELAMENTO, 0, protocolo, cancelada.matricula,
                               0, cancelada.nome_solicitante);
    }
    else // Protocolo inexistente ou fila vazia.
    {
        printf("Erro: protocolo %d nao encontrado na fila.\n", protocolo);
    }
}

void mostrar_estado(Sistema *s)
{
    printf("\n=== Equipamentos ===\n");
    listar_equipamentos(s->equipamentos);
    printf("\n=== Fila de solicitacoes ===\n");
    listar_solicitacoes(&s->fila);
    printf("\n=== Operacoes recentes (topo primeiro) ===\n");
    listar_operacoes(s->operacoes);
    printf("\n=== Historico de movimentacoes ===\n");
    listar_historico(s->historico);
}

void salvar_dados(Sistema *s)
{
    int eq = salvar_equipamentos(ARQ_EQUIPAMENTOS, s->equipamentos); // Grava os equipamentos.
    int mov = salvar_historico(ARQ_HISTORICO, s->historico); // Grava o historico.

    if (eq < 0 || mov < 0) // Falha ao abrir ou gravar algum arquivo.
    {
        printf("Erro: nao foi possivel salvar os dados.\n");
    }
    else
    {
        printf("Dados salvos: %d equipamentos, %d movimentacoes.\n", eq, mov);
    }
}

void carregar_dados(Sistema *s)
{
    int eq = carregar_equipamentos(ARQ_EQUIPAMENTOS, &s->equipamentos); // Le os equipamentos.
    int mov = carregar_historico(ARQ_HISTORICO, &s->historico); // Le o historico.

    if (eq < 0 || mov < 0) // Arquivo inexistente ou que nao abriu.
    {
        printf("Erro: nao foi possivel abrir os arquivos de dados.\n");
    }
    else
    {
        printf("Dados carregados: %d equipamentos, %d movimentacoes.\n", eq, mov);
    }
}

void liberar_sistema(Sistema *s)
{
    liberar_equipamentos(&s->equipamentos); // Libera cada estrutura dinamica.
    liberar_fila(&s->fila);
    liberar_operacoes(&s->operacoes);
    liberar_historico(&s->historico);
    printf("Memoria liberada. Encerrando o sistema.\n");
}

int main(void)
{
    Sistema s;
    memset(&s, 0, sizeof(Sistema)); // Tudo comeca vazio (ponteiros NULL e contadores zerados).
    s.proximo_protocolo = 1; // O primeiro protocolo e 1.
    ler_texto("Data de hoje (dd/mm/aaaa): ", s.data, TAM_DATA_OP); // Data usada nas operacoes.

    int opcao = -1;

    while (opcao != 0) // Menu em loop, encerrado pela opcao 0.
    {
        mostrar_menu();
        opcao = ler_inteiro("Opcao: ");

        if (opcao == 1) cadastrar_equipamento(&s);
        else if (opcao == 2) listar_equipamentos(s.equipamentos);
        else if (opcao == 3) buscar_equipamento_menu(&s);
        else if (opcao == 4) registrar_solicitacao(&s);
        else if (opcao == 5) listar_solicitacoes(&s.fila);
        else if (opcao == 6) atender_proxima(&s);
        else if (opcao == 7) registrar_devolucao(&s);
        else if (opcao == 8) alterar_manutencao_menu(&s);
        else if (opcao == 9) cancelar_solicitacao_menu(&s);
        else if (opcao == 10) mostrar_estado(&s);
        else if (opcao == 11) salvar_dados(&s);
        else if (opcao == 12) carregar_dados(&s);
        else if (opcao == 0)
        {
            char resposta[8];
            ler_texto("Deseja salvar antes de sair? (s/n): ", resposta, sizeof(resposta));
            if (resposta[0] == 's' || resposta[0] == 'S')
            {
                salvar_dados(&s);
            }
        }
        else printf("Erro: opcao invalida.\n");
    }

    liberar_sistema(&s); // Libera toda a memoria antes de encerrar.
    return 0;
}