# O controle da lampisteria

Sistema de controle de equipamentos de mineração (lanternas, autorresgatadores, detectores de gás e rádios), em linguagem C.

- **Aluno:** Felipe Natan Beirigo Barbosa
- **Faculdade:** UFCI-MG, Unidade Divinópolis
- **Disciplina:** IPC, Introdução à Programação de Computadores
- **Professor:** Guido Pantuza
- **Trabalho:** 2º, "O controle da lampisteria"

## Como compilar e executar

Desenvolvido no Windows, com o `gcc` no terminal do VS Code. Na pasta do projeto:

```bash
gcc -Wall -Wextra -o lampisteria main.c equipamentos.c solicitacoes.c
.\lampisteria
```

A cada etapa nova, o módulo correspondente é acrescentado ao comando.

## Estruturas de dados

- **Equipamentos:** lista encadeada, em ordem crescente de identificador.
- **Solicitações:** fila (FIFO), com ponteiros para o início e o fim.
- **Operações recentes:** pilha. *(em desenvolvimento)*
- **Histórico:** lista encadeada. *(em desenvolvimento)*

## Progresso

- [x] Estrutura do projeto e arquivos dos módulos
- [x] Equipamentos: struct, busca por id, inserção ordenada e listagem
- [x] Solicitações: struct, fila, enfileirar, desenfileirar, listar, cancelar e liberar
- [ ] Equipamentos: alterar estado, listar por categoria e liberar
- [ ] Pilha de operações recentes
- [ ] Histórico de movimentações
- [ ] Arquivos binários (salvar e carregar)
- [ ] Menu e lógica de atendimento
- [ ] Testes com os exemplos do enunciado

## Decisões de projeto

- Estado do equipamento com `#define` e `int`, sem `enum`.
- Textos em vetores de `char`, com tamanhos definidos por constantes. A data é texto (`dd/mm/aaaa`).
- `inserir_equipamento` recebe `Equipamento **inicio`, porque pode trocar o primeiro nó da lista.
- A `Fila` guarda `inicio`, `fim` e `tamanho`.
- `desenfileirar` e `cancelar_solicitacao` copiam os dados do nó removido para quem chamou, para o histórico poder registrar nome e matrícula.

## Testes

Feitos até agora:

- Cadastro fora de ordem (102 e depois 101): a listagem mostra o 101 primeiro.
- Compilação dos módulos de equipamentos e solicitações, sem erros.