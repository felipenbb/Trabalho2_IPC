#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"

int salvar_equipamentos(const char *arquivo, Equipamento *inicio)
{
    FILE *f = fopen(arquivo, "wb"); // Abre para escrita binaria.
    if (f == NULL) // Falha na abertura do arquivo.
    {
        return -1;
    }

    int total = 0;
    Equipamento *atual = inicio; // Ponteiro auxiliar que percorre a lista.

    while (atual)
    {
        Equipamento copia = *atual; // Copia o no para nao alterar a lista.
        copia.prox = NULL; // Ponteiro nao e informacao persistente, entao nao grava o endereco.

        if (fwrite(&copia, sizeof(Equipamento), 1, f) != 1) // Grava um equipamento por vez.
        {
            fclose(f);
            return -1;
        }
        total++;
        atual = atual->prox;
    }

    fclose(f);
    return total;
}

int carregar_equipamentos(const char *arquivo, Equipamento **inicio)
{
    FILE *f = fopen(arquivo, "rb"); // Abre para leitura binaria.
    if (f == NULL) // Falha na abertura do arquivo.
    {
        return -1;
    }

    liberar_equipamentos(inicio); // Libera a lista atual antes de receber os dados do arquivo.

    Equipamento lido;
    Equipamento *fim = NULL; // Ultimo no da nova lista, para manter a ordem do arquivo.
    int total = 0;

    while (fread(&lido, sizeof(Equipamento), 1, f) == 1) // Le um equipamento por vez ate o fim do arquivo.
    {
        Equipamento *novo = malloc(sizeof(Equipamento)); // Recria o no dinamicamente.
        if (novo == NULL)
        {
            printf("Erro: Memoria insuficiente\n");
            break;
        }

        *novo = lido;
        novo->prox = NULL; // Religa os ponteiros na ordem original.

        if (*inicio == NULL)
        {
            *inicio = novo;
        }
        else
        {
            fim->prox = novo;
        }
        fim = novo;
        total++;
    }

    fclose(f);
    return total;
}

int salvar_historico(const char *arquivo, Movimentacao *inicio)
{
    FILE *f = fopen(arquivo, "wb"); // Abre para escrita binaria.
    if (f == NULL) // Falha na abertura do arquivo.
    {
        return -1;
    }

    int total = 0;
    Movimentacao *atual = inicio; // Ponteiro auxiliar que percorre a lista.

    while (atual)
    {
        Movimentacao copia = *atual; // Copia o no para nao alterar a lista.
        copia.prox = NULL; // Nao grava o endereco do proximo.

        if (fwrite(&copia, sizeof(Movimentacao), 1, f) != 1) // Grava uma movimentacao por vez.
        {
            fclose(f);
            return -1;
        }
        total++;
        atual = atual->prox;
    }

    fclose(f);
    return total;
}

int carregar_historico(const char *arquivo, Movimentacao **inicio)
{
    FILE *f = fopen(arquivo, "rb"); // Abre para leitura binaria.
    if (f == NULL) // Falha na abertura do arquivo.
    {
        return -1;
    }

    liberar_historico(inicio); // Libera o historico atual antes de receber os dados do arquivo.

    Movimentacao lido;
    Movimentacao *fim = NULL; // Ultimo no da nova lista, para manter a ordem cronologica.
    int total = 0;

    while (fread(&lido, sizeof(Movimentacao), 1, f) == 1) // Le uma movimentacao por vez ate o fim do arquivo.
    {
        Movimentacao *novo = malloc(sizeof(Movimentacao)); // Recria o no dinamicamente.
        if (novo == NULL)
        {
            printf("Erro: Memoria insuficiente\n");
            break;
        }

        *novo = lido;
        novo->prox = NULL; // Religa os ponteiros na ordem original.

        if (*inicio == NULL)
        {
            *inicio = novo;
        }
        else
        {
            fim->prox = novo;
        }
        fim = novo;
        total++;
    }

    fclose(f);
    return total;
}