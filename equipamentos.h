#ifndef EQUIPAMENTOS_H
#define EQUIPAMENTOS_H

#define TAM_DESCRICAO 50               // Tamanhos de cada propriedade do equipamento
#define TAM_CATEGORIA 30
#define TAM_PATRIMONIO 20
#define TAM_NOME 50

#define DISPONIVEL 0                   //Define o estado do equipamento
#define EMPRESTADO 1
#define MANUTENCAO 2

typedef struct Equipamento 
{
    int id;                            // Caracteristicas do equipamentos
    char descricao[TAM_DESCRICAO];
    char categoria[TAM_CATEGORIA];
    char patrimonio[TAM_PATRIMONIO];
    int estado;
    char nome_responsavel [TAM_NOME];
    int matricula_responsavel;
    
    struct Equipamento *prox;         	// Ponteiro que aponta para o proximo equipamento.
} Equipamento;

	int inserir_equipamento(Equipamento **inicio, Equipamento dados);
	Equipamento *buscar_equipamento(Equipamento *inicio, int id);
	void listar_equipamentos(Equipamento *inicio);

#endif