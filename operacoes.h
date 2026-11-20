#ifndef OPERACOES_H
#define OPERACOES_H

#define TAM_DESC_OP 100 // Tamanhos de cada propriedade da operacao.
#define TAM_DATA_OP 12

typedef struct Operacao
{
    int numero; // Ordem em que a operacao foi feita.
    char descricao[TAM_DESC_OP]; // Texto da acao realizada.
    char data[TAM_DATA_OP]; // Texto no formato dd/mm/aaaa.
    struct Operacao *prox; // Aponta para a operacao anterior (a pilha cresce para cima).
} Operacao;

int empilhar_operacao(Operacao **topo, const char *descricao, const char *data);
void listar_operacoes(Operacao *topo);
void liberar_operacoes(Operacao **topo);

#endif