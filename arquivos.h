#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "equipamentos.h"
#include "historico.h"

#define ARQ_EQUIPAMENTOS "equipamentos.dat" // Nomes dos arquivos binarios.
#define ARQ_HISTORICO "historico.dat"

int salvar_equipamentos(const char *arquivo, Equipamento *inicio); // Devolve quantos gravou ou -1 se falhar.
int carregar_equipamentos(const char *arquivo, Equipamento **inicio); // Devolve quantos leu ou -1 se nao abrir o arquivo.
int salvar_historico(const char *arquivo, Movimentacao *inicio); // Devolve quantos gravou ou -1 se falhar.
int carregar_historico(const char *arquivo, Movimentacao **inicio); // Devolve quantos leu ou -1 se nao abrir o arquivo.

#endif