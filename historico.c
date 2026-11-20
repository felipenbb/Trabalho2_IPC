#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historico.h"

int inserir_movimentacao(Movimentacao **inicio, Movimentacao dados)
{
    Movimentacao *novo = malloc(sizeof(Movimentacao)); // Reserva memoria para o novo no.
    if (novo == NULL) // Verifica se a memoria foi alocada.
    {
        printf("Erro: Memoria insuficiente\n");
        return 0;
    }

    *novo = dados; // Copia todos os campos de dados para o novo no.
    novo->prox = NULL; // O novo no sera o ultimo, entao nao aponta para ninguem.

    if (*inicio == NULL) // Lista vazia: o novo no e o primeiro.
    {
        *inicio = novo;
        return 1;
    }

    Movimentacao *atual = *inicio; // Ponteiro auxiliar que percorre a lista.

    while (atual->prox != NULL) // Avanca ate o ultimo no.
    {
        atual = atual->prox;
    }

    atual->prox = novo; // O antigo ultimo aponta para o novo.

    return 1;
}

void listar_historico(Movimentacao *inicio)
{
    if (inicio == NULL) // Lista vazia, nao ha o que mostrar.
    {
        printf("Nenhuma movimentacao registrada.\n");
        return;
    }

    Movimentacao *atual = inicio; // Ponteiro auxiliar que percorre a lista, comecando pelo primeiro.
    int posicao = 1; // Numero da movimentacao na listagem.

    while (atual)
    {
        if (atual->tipo == MOV_EMPRESTIMO) // Cada tipo mostra os dados que fazem sentido para ele.
        {
            printf("%d. Emprestimo | equipamento %d | %s (%d) | protocolo %d\n",
                   posicao, atual->id_equipamento, atual->nome_responsavel,
                   atual->matricula, atual->protocolo);
        }
        else if (atual->tipo == MOV_DEVOLUCAO)
        {
            printf("%d. Devolucao | equipamento %d | %s (%d)\n",
                   posicao, atual->id_equipamento, atual->nome_responsavel,
                   atual->matricula);
        }
        else if (atual->tipo == MOV_MANUTENCAO)
        {
            printf("%d. Manutencao | equipamento %d | %s\n",
                   posicao, atual->id_equipamento,
                   atual->entrada_manutencao ? "entrada" : "saida");
        }
        else
        {
            printf("%d. Cancelamento | protocolo %d | %s (%d)\n",
                   posicao, atual->protocolo, atual->nome_responsavel,
                   atual->matricula);
        }

        atual = atual->prox; // Passa para a proxima movimentacao.
        posicao++; // Atualiza o numero para a proxima linha.
    }
}

void liberar_historico(Movimentacao **inicio)
{
    Movimentacao *atual = *inicio; // Comeca pelo primeiro.

    while (atual)
    {
        Movimentacao *proximo = atual->prox; // Guarda o proximo antes de liberar.
        free(atual); // Libera o no atual.
        atual = proximo; // Passa para o no seguinte.
    }

    *inicio = NULL; // Deixa a lista vazia depois de liberar.
}