#include <stdio.h>
#include "equipamentos.h"

int main(void)
{
    Equipamento *lista = NULL;
    Equipamento e;

    e.id = 102;
    e.estado = DISPONIVEL;
    e.matricula_responsavel = 0;
    e.nome_responsavel[0] = '\0';
    snprintf(e.descricao, TAM_DESCRICAO, "Autorresgatador W65");
    snprintf(e.categoria, TAM_CATEGORIA, "Autorresgatador");
    snprintf(e.patrimonio, TAM_PATRIMONIO, "PAT-0102");
    inserir_equipamento(&lista, e);

    e.id = 101;
    snprintf(e.descricao, TAM_DESCRICAO, "Lanterna de capacete LED");
    snprintf(e.categoria, TAM_CATEGORIA, "Lanterna");
    snprintf(e.patrimonio, TAM_PATRIMONIO, "PAT-0101");
    inserir_equipamento(&lista, e);

    listar_equipamentos(lista);

    return 0;
}