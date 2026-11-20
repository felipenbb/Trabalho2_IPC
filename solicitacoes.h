#ifndef SOLICITACOES_H
#define SOLICITACOES_H

#include "equipamentos.h" // Reaproveita TAM_NOME e TAM_CATEGORIA.

#define TAM_LOCAL 50 
#define TAM_DATA 12

typedef struct Solicitacao 
{
    int protocolo; 
    int matricula;

    char nome_solicitante[TAM_NOME]; 
    char categoria[TAM_CATEGORIA];
    char local[TAM_LOCAL];
    char data[TAM_DATA]; 
    struct Solicitacao *prox; // Ponteiro que aponta para a proxima solicitacao da fila.
} Solicitacao;

typedef struct 
{
    Solicitacao *inicio; 
    Solicitacao *fim; 
    int tamanho;
} Fila;

void enfileirar(Fila *fila, Solicitacao dados);
int desenfileirar(Fila *fila, Solicitacao *removida);
void listar_solicitacoes(Fila *fila);
int cancelar_solicitacao(Fila *fila, int protocolo, Solicitacao *cancelada);
void liberar_fila(Fila *fila);
#endif