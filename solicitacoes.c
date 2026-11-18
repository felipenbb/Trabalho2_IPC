#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solicitacoes.h"

void enfileirar(Fila *fila, Solicitacao dados)
{
    Solicitacao *novo = malloc(sizeof(Solicitacao)); // Reserva memoria para o novo no.
    if (novo == NULL) // Verifica se a memoria foi alocada.
    {
        printf("Erro: Memoria insuficiente\n");
        return;
    }

    *novo = dados; // Copia todos os campos de dados para o novo no.
    novo->prox = NULL; // O novo no sera o ultimo, entao nao aponta para ninguem.

    if(fila->inicio == NULL) // Fila vazia: o novo no e o primeiro e o ultimo ao mesmo tempo.
    {
        fila->inicio = novo;
        fila->fim = novo;
    }
    else // Fila com elementos: o antigo ultimo aponta para o novo, que passa a ser o ultimo.
    {
        fila->fim->prox = novo; // A ordem importa: se atualizar o fim antes, o antigo ultimo nao aponta para o novo.
        fila->fim = novo;
    }

    fila->tamanho++; // Aumenta a quantidade de solicitacoes na fila.

}

int desenfileirar(Fila *fila, Solicitacao *removida)
{
    if (fila->inicio == NULL) // Fila vazia
    {
        printf("Nada a remover");
        return 0;
    }

    Solicitacao *remover = fila->inicio; // Guarda o no que vai sair.

    *removida = *remover; // Entrega os dados de quem saiu para quem chamou a funcao.

    fila->inicio = remover->prox; // O segundo da fila passa a ser o primeiro.

    if (fila->inicio == NULL) // A fila esvaziou, tem que apontar pra null.
    {
        fila->fim = NULL;
    }

    free(remover); // Libera a memoria
    fila->tamanho--; // Atualiza o contador.

    return 1;
}
void listar_solicitacoes(Fila *fila)
{
    if (fila->inicio == NULL) // Fila vazia
    {
        printf("Fila de solicitacoes vazia.\n");
        return;
    }

    Solicitacao *atual = fila->inicio; // Ponteiro auxiliar que percorre a fila, comecando pelo primeiro.
    int posicao = 1; // Numero da posicao na fila, para mostrar 1o, 2o, 3o...

    printf("Fila de solicitacoes (%d):\n", fila->tamanho); // Cabecalho impresso uma vez so, antes do laco.

    while (atual)
    {
        printf("%do | Protocolo %d | %s (%d) | %s | %s\n",
               posicao, atual->protocolo, atual->nome_solicitante,
               atual->matricula, atual->categoria, atual->local);

        atual = atual->prox; // Passa pra proxima solicitacao
        posicao++; // Atualiza o numero da posicao para a proxima linha.
    }
}
int cancelar_solicitacao(Fila *fila, int protocolo, Solicitacao *cancelada)
{
    if (fila->inicio == NULL) // Fila vazia.
    {
        return 0;
    }

    if (fila->inicio->protocolo == protocolo) // Se for o primeiro da fila, e o mesmo caso do desenfileirar.
    {
        return desenfileirar(fila, cancelada);
    }

    Solicitacao *anterior = fila->inicio; // Ponteiro auxiliar que vai parar no no anterior ao procurado.

    while (anterior->prox != NULL && anterior->prox->protocolo != protocolo) // Avanca ate o proximo ser o protocolo procurado.
    {
        anterior = anterior->prox;
    }

    if (anterior->prox == NULL) // Chegou ao fim sem achar o protocolo.
    {
        return 0;
    }

    Solicitacao *remover = anterior->prox; // No que vai ser cancelado.

    *cancelada = *remover; // Entrega os dados do cancelado para quem chamou a funcao.

    anterior->prox = remover->prox; // O anterior pula o removido e aponta para o que vinha depois.

    if (remover == fila->fim) // Se o removido era o ultimo, o anterior passa a ser o fim.
    {
        fila->fim = anterior;
    }

    free(remover);
    fila->tamanho--;

    return 1;
}

void liberar_fila(Fila *fila)
{
    Solicitacao *atual = fila->inicio; // Comeca pelo primeiro da fila.

    while (atual)
    {
        Solicitacao *proximo = atual->prox; // Guarda o proximo antes de liberar, senao o endereco se perde.
        free(atual); // Libera o no atual.
        atual = proximo; // Passa para o no seguinte.
    }

    fila->inicio = NULL; // Deixando a fila vazia, pra ter certeza que nao ha lixo de memoria
    fila->fim = NULL;
    fila->tamanho = 0;
}