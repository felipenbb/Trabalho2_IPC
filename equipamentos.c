#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipamentos.h"

Equipamento *buscar_equipamento(Equipamento *inicio, int id) 
{
    Equipamento *atual = inicio; // Ponteiro auxiliar que percorre a lista, comecando pelo primeiro.
    while(atual)
    {
        if (atual->id == id) // Verifica se o ID do equipamento ja existe e retorna caso exista.
        {
            return atual;
        }
        atual = atual->prox; // Se nao existir passa pro proximo.
    }
    
    return NULL; // Chegou ao fim da lista sem achar o ID.
}

int inserir_equipamento(Equipamento **inicio, Equipamento dados)
{

    if (buscar_equipamento(*inicio, dados.id) != NULL) // Verifica equipamentos repetidos
    {
        return 0; // Recusa a insercao se o ID ja existe.
    }

    Equipamento *novo = malloc(sizeof(Equipamento)); // Reserva memoria para o novo no.
    if (novo == NULL) // Verifica se a memoria foi alocada.
    {
        printf("Erro: Memória insuficiente\n");
        return 0;
    }

    *novo = dados; // Copia todos os campos de dados para o novo no.

    novo->prox = NULL; // O novo no ainda nao aponta para ninguem.

    if (*inicio == NULL || dados.id < (*inicio)->id) // Lista vazia ou ID menor que o do primeiro: entra no inicio.
    {
        novo->prox = *inicio; // O novo no aponta para o antigo primeiro (ou NULL se a lista estava vazia).
        *inicio = novo; // O novo no passa a ser o primeiro da lista.
        return 1;
    }

    Equipamento *atual = *inicio; // Ponteiro auxiliar que percorre a lista, comecando pelo primeiro.

    while (atual->prox != NULL && atual->prox->id < dados.id) // Avanca ate parar no no anterior a posicao correta.
    {
        atual = atual->prox;
    }

    novo->prox = atual->prox; // O novo no aponta para o no que vinha depois de atual.
    atual->prox = novo; // Atual passa a apontar para o novo no (a ordem evita perder o resto da lista).

    return 1; // Insercao concluida, mantendo a ordem crescente de ID.
}

void imprimir_equipamento(Equipamento *atual) // Imprime uma linha com os dados de um equipamento.
{
    printf("%d | %s | %s | %s | ", atual->id, atual->descricao,
           atual->categoria, atual->patrimonio);

    if (atual->estado == DISPONIVEL) // Mostra o estado em texto.
    {
        printf("Disponivel | ");
    }
    else if (atual->estado == EMPRESTADO)
    {
        printf("Emprestado | ");
    }
    else
    {
        printf("Manutencao | ");
    }

    if (atual->estado == EMPRESTADO) // So mostra o responsavel se estiver emprestado.
    {
        printf("%s (%d)\n", atual->nome_responsavel, atual->matricula_responsavel);
    }
    else
    {
        printf("-\n");
    }
}

void listar_equipamentos(Equipamento *inicio)
{
    if (inicio == NULL) // Lista vazia, nao ha o que mostrar.
    {
        printf("Nenhum equipamento cadastrado.\n");
        return;
    }

    printf("ID | Descricao | Categoria | Patrimonio | Estado | Responsavel\n");

    Equipamento *atual = inicio; // Ponteiro auxiliar que percorre a lista.

    while (atual)
    {
        imprimir_equipamento(atual);
        atual = atual->prox; // Passa pro proximo equipamento.
    }
}

void listar_por_categoria(Equipamento *inicio, const char *categoria)
{
    Equipamento *atual = inicio; // Ponteiro auxiliar que percorre a lista.
    int encontrados = 0; // Conta quantos equipamentos da categoria foram achados.

    while (atual)
    {
        if (strcmp(atual->categoria, categoria) == 0) // Compara os textos (0 significa iguais).
        {
            if (encontrados == 0) // Imprime o cabecalho so antes do primeiro achado.
            {
                printf("ID | Descricao | Categoria | Patrimonio | Estado | Responsavel\n");
            }
            imprimir_equipamento(atual);
            encontrados++;
        }
        atual = atual->prox;
    }

    if (encontrados == 0) // Nenhum equipamento da categoria.
    {
        printf("Nenhum equipamento da categoria %s.\n", categoria);
    }
}

Equipamento *emprestar_equipamento(Equipamento *inicio, const char *categoria, const char *nome, int matricula)
{
    Equipamento *atual = inicio; // Percorre a lista em ordem de id, entao acha o primeiro disponivel.

    while (atual)
    {
        if (atual->estado == DISPONIVEL && strcmp(atual->categoria, categoria) == 0) // Disponivel e da categoria pedida.
        {
            atual->estado = EMPRESTADO; // Marca como emprestado.
            strncpy(atual->nome_responsavel, nome, TAM_NOME - 1); // Guarda o responsavel sem passar do vetor.
            atual->nome_responsavel[TAM_NOME - 1] = '\0';
            atual->matricula_responsavel = matricula;
            return atual;
        }
        atual = atual->prox;
    }

    return NULL; // Nao ha equipamento disponivel da categoria.
}

int solicitante_possui_categoria(Equipamento *inicio, int matricula, const char *categoria)
{
    Equipamento *atual = inicio; // Ponteiro auxiliar que percorre a lista.

    while (atual)
    {
        if (atual->estado == EMPRESTADO && atual->matricula_responsavel == matricula
            && strcmp(atual->categoria, categoria) == 0) // Emprestado a essa matricula e da mesma categoria.
        {
            return 1;
        }
        atual = atual->prox;
    }

    return 0; // Nao possui nenhum da categoria.
}

int devolver_equipamento(Equipamento *inicio, int id, Equipamento *devolvido)
{
    Equipamento *equip = buscar_equipamento(inicio, id); // Reaproveita a busca por id.

    if (equip == NULL) // Equipamento inexistente.
    {
        return 0;
    }

    if (equip->estado != EMPRESTADO) // So pode devolver o que esta emprestado.
    {
        return -1;
    }

    *devolvido = *equip; // Guarda os dados (com o responsavel) para a pilha e o historico.

    equip->estado = DISPONIVEL; // Volta a ficar disponivel.
    equip->nome_responsavel[0] = '\0'; // Limpa o responsavel.
    equip->matricula_responsavel = 0;

    return 1;
}

int alterar_manutencao(Equipamento *inicio, int id)
{
    Equipamento *equip = buscar_equipamento(inicio, id); // Reaproveita a busca por id.

    if (equip == NULL) // Equipamento inexistente.
    {
        return 0;
    }

    if (equip->estado == EMPRESTADO) // Emprestado nao pode entrar em manutencao.
    {
        return -1;
    }

    if (equip->estado == DISPONIVEL) // Disponivel vai para manutencao.
    {
        equip->estado = MANUTENCAO;
        return 1;
    }

    equip->estado = DISPONIVEL; // Em manutencao volta a ficar disponivel.
    return 2;
}

void liberar_equipamentos(Equipamento **inicio)
{
    Equipamento *atual = *inicio; // Comeca pelo primeiro.

    while (atual)
    {
        Equipamento *proximo = atual->prox; // Guarda o proximo antes de liberar.
        free(atual); // Libera o no atual.
        atual = proximo; // Passa para o no seguinte.
    }

    *inicio = NULL; // Deixa a lista vazia depois de liberar.
}
