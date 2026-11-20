#ifndef HISTORICO_H
#define HISTORICO_H

#include "equipamentos.h" // Reaproveita TAM_NOME.

#define MOV_EMPRESTIMO 0 // Tipos de movimentacao.
#define MOV_DEVOLUCAO 1
#define MOV_MANUTENCAO 2
#define MOV_CANCELAMENTO 3

typedef struct Movimentacao
{
    int tipo; // Um dos tipos definidos acima.
    int id_equipamento; // Equipamento envolvido (0 se nao houver, como no cancelamento).
    int protocolo; // Protocolo da solicitacao (0 se nao houver).
    int matricula; // Matricula de quem foi atendido ou cancelado (0 se nao houver).
    int entrada_manutencao; // 1 se o equipamento entrou em manutencao, 0 se saiu.
    char nome_responsavel[TAM_NOME]; // Nome de quem foi atendido ou cancelado.
    struct Movimentacao *prox; // Aponta para a proxima movimentacao.
} Movimentacao;

int inserir_movimentacao(Movimentacao **inicio, Movimentacao dados); // Insere no fim da lista.
void listar_historico(Movimentacao *inicio); // Mostra em ordem cronologica.
void liberar_historico(Movimentacao **inicio); // Libera toda a lista.

#endif