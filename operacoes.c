#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operacoes.h"

int empilhar_operacao(Operacao **topo, const char *descricao, const char *data)
{
    Operacao *novo = malloc(sizeof(Operacao)); // Reserva memoria para o novo no.
    if (novo == NULL) // Verifica se a memoria foi alocada.
    {
        printf("Erro: Memoria insuficiente\n");
        return 0;
    }

    if (*topo == NULL) // Pilha vazia: a primeira operacao e a de numero 1.
    {
        novo->numero = 1;
    }
    else // Senao, o numero e o do topo atual mais 1.
    {
        novo->numero = (*topo)->numero + 1;
    }

    strncpy(novo->descricao, descricao, TAM_DESC_OP - 1); // Copia sem passar do tamanho do vetor.
    novo->descricao[TAM_DESC_OP - 1] = '\0'; // Garante o fim do texto.
    strncpy(novo->data, data, TAM_DATA_OP - 1);
    novo->data[TAM_DATA_OP - 1] = '\0';

    novo->prox = *topo; // O novo no aponta para o antigo topo.
    *topo = novo; // O novo no passa a ser o topo da pilha.

    return 1; // Operacao empilhada.
}

void listar_operacoes(Operacao *topo)
{
    if (topo == NULL) // Pilha vazia, nao ha o que mostrar.
    {
        printf("Nenhuma operacao registrada.\n");
        return;
    }

    Operacao *atual = topo; // Ponteiro auxiliar que percorre a pilha, comecando pelo topo.

    while (atual)
    {
        printf("%d. %s\n", atual->numero, atual->descricao);
        atual = atual->prox; // Desce para a operacao anterior.
    }
}

void liberar_operacoes(Operacao **topo)
{
    Operacao *atual = *topo; // Comeca pelo topo.

    while (atual)
    {
        Operacao *proximo = atual->prox; // Guarda o proximo antes de liberar, senao o endereco se perde.
        free(atual); // Libera o no atual.
        atual = proximo; // Passa para o no seguinte.
    }

    *topo = NULL; // Deixa a pilha vazia depois de liberar.
}