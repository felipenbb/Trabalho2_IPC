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
    void listar_por_categoria(Equipamento *inicio, const char *categoria); // Lista so os equipamentos da categoria.
    Equipamento *emprestar_equipamento(Equipamento *inicio, const char *categoria, const char *nome, int matricula); // Devolve o equipamento emprestado ou NULL se nao houver disponivel.
    int solicitante_possui_categoria(Equipamento *inicio, int matricula, const char *categoria); // Devolve 1 se a matricula ja tem equipamento emprestado da categoria.
    int devolver_equipamento(Equipamento *inicio, int id, Equipamento *devolvido); // Devolve 1 (ok), 0 (nao encontrado) ou -1 (nao esta emprestado).
    int alterar_manutencao(Equipamento *inicio, int id); // Devolve 1 (entrou em manutencao), 2 (voltou a disponivel), 0 (nao encontrado) ou -1 (emprestado).
    void liberar_equipamentos(Equipamento **inicio); // Libera toda a lista.
    void imprimir_equipamento(Equipamento *atual); // Imprime uma linha com os dados de um equipamento.
#endif