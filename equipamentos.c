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

void listar_equipamentos(Equipamento *inicio)
{
    if (inicio == NULL) // Lista vazia, nao ha o que mostrar.
    {
        printf("Nenhum equipamento cadastrado.\n");
        return;
    }

    printf("ID | Descricao | Categoria | Patrimonio | Estado | Responsavel\n");

    Equipamento *atual = inicio; // Ponteiro que percorre a lista.

    while (atual)
    {
        printf("%d | %s | %s | %s | ", atual->id, atual->descricao, atual->categoria, atual->patrimonio);

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

        atual = atual->prox; // Passa pro proximo equipamento.
    }
}