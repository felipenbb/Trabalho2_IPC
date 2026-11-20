# O controle da lampisteria

Sistema de controle de equipamentos de mineração (lanternas, autorresgatadores, detectores de gás e rádios), desenvolvido em linguagem C.

- Aluno: Felipe Natan Beirigo Barbosa
- Faculdade: UFCI-MG, Unidade Divinópolis
- Disciplina: IPC, Introdução à Programação de Computadores
- Professor: Guido Pantuza
- Trabalho: 2º, "O controle da lampisteria"

## Descrição

O sistema mantém o cadastro dos equipamentos, recebe as solicitações dos mineiros em uma fila, atende cada solicitação emprestando o primeiro equipamento disponível da categoria pedida, registra devoluções e manutenções, guarda as operações recentes em uma pilha, mantém o histórico das movimentações concluídas e salva e carrega os dados em arquivos binários.

## Como compilar e executar

Desenvolvido no Windows, com o `gcc` no terminal do VS Code, e testado também em Linux.

Na pasta do projeto, execute:

```bash
gcc -Wall -Wextra -o lampisteria main.c equipamentos.c solicitacoes.c operacoes.c historico.c arquivos.c
```

Executar no Windows (PowerShell):

```powershell
.\lampisteria
```

Executar no Linux:

```bash
./lampisteria
```

A compilação não gera erros nem warnings.

## Como usar

1. Ao iniciar, o programa pede a data de hoje (`dd/mm/aaaa`), que é usada nas operações registradas.
2. Em seguida, aparece o menu, em loop, que só termina na opção 0.
3. Digite a opção e responda o que o programa pedir, um dado por linha.

| Opção | Operação |
|---|---|
| 1 | Cadastrar equipamento (recusa identificador repetido) |
| 2 | Listar equipamentos |
| 3 | Buscar equipamento por identificador ou listar por categoria |
| 4 | Registrar solicitação (gera o protocolo e entra no fim da fila) |
| 5 | Listar solicitações pendentes |
| 6 | Atender próxima solicitação |
| 7 | Registrar devolução |
| 8 | Alterar manutenção (disponível para manutenção e vice-versa) |
| 9 | Cancelar solicitação pelo protocolo |
| 10 | Mostrar estado do sistema (equipamentos, fila, pilha e histórico) |
| 11 | Salvar dados |
| 12 | Carregar dados |
| 0 | Sair (pergunta se deseja salvar e libera a memória) |

A categoria é comparada pelo texto exato: "Lanterna" e "lanterna" são categorias diferentes.

## Estruturas de dados

| Estrutura | Tipo | Uso |
|---|---|---|
| Equipamentos | Lista encadeada, em ordem crescente de identificador | Cadastro e estado de cada equipamento |
| Solicitações | Fila (FIFO), com ponteiros para início e fim | Pedidos pendentes, atendidos por ordem de chegada |
| Operações recentes | Pilha (a mais recente é o topo) | Ações realizadas com sucesso |
| Histórico | Lista encadeada, em ordem cronológica | Movimentações concluídas |

## Organização dos arquivos

| Arquivo | Responsabilidade |
|---|---|
| `main.c` | Menu e funções de entrada e saída; liga os módulos |
| `equipamentos.h` / `.c` | Cadastro, busca, listagem, empréstimo, devolução, manutenção e liberação |
| `solicitacoes.h` / `.c` | Fila: enfileirar, desenfileirar, listar, cancelar e liberar |
| `operacoes.h` / `.c` | Pilha: empilhar, listar e liberar |
| `historico.h` / `.c` | Histórico: inserir, listar e liberar |
| `arquivos.h` / `.c` | Salvar e carregar os arquivos binários |

## Regras de funcionamento

- Não pode haver dois equipamentos com o mesmo identificador.
- A solicitação entra no fim da fila e é atendida por ordem de chegada.
- O pedido é recusado se o solicitante (mesma matrícula) já tiver um equipamento da mesma categoria emprestado.
- No atendimento, é emprestado o primeiro equipamento disponível da categoria, em ordem de identificador.
- Sem equipamento disponível, é possível manter a solicitação no início da fila, recolocá-la no fim ou cancelá-la.
- Equipamento emprestado não pode ser emprestado de novo nem enviado para manutenção.
- A devolução só vale para equipamento existente e emprestado.
- Toda operação realizada com sucesso entra na pilha. Tentativas recusadas não entram.
- Toda movimentação concluída (empréstimo, devolução, manutenção e cancelamento) entra no histórico.

## Decisões de projeto

- Estados com `#define`: o estado do equipamento e o tipo de movimentação são constantes definidas com `#define` e guardadas em `int`.
- Textos em vetores de `char`: tamanhos definidos por constantes. A data é texto no formato `dd/mm/aaaa`.
- Responsável pelo empréstimo: o equipamento guarda `nome_responsavel` e `matricula_responsavel`, que só valem quando está emprestado e são limpos na devolução.
- Inserção ordenada: `inserir_equipamento` recebe `Equipamento **inicio`, porque pode trocar o primeiro nó da lista.
- Fila com início, fim e tamanho: o `fim` evita percorrer a fila a cada inserção, e o `tamanho` informa a posição na fila.
- Dados devolvidos pelas remoções: `desenfileirar`, `cancelar_solicitacao` e `devolver_equipamento` copiam os dados do nó removido para quem chamou, para a pilha e o histórico registrarem nome e matrícula.
- Atendimento: o primeiro da fila só é removido se houver equipamento disponível. Por isso, "manter no início" não altera a fila, e "recolocar no final" remove e enfileira de novo.
- Estado do sistema agrupado: a `struct Sistema` no `main.c` reúne a lista, a fila, a pilha, o histórico e o contador de protocolo, para passar um ponteiro só às funções.
- Numeração da pilha: cada operação guarda o próprio número (1, 2, 3...), calculado a partir do topo.
- Menu sem lógica de negócio: o `main.c` só lê dados e chama as funções dos módulos.
- Memória: todo nó é criado com `malloc`, e cada módulo tem uma função de liberação, chamadas antes de encerrar.

## Arquivos binários

- São salvos `equipamentos.dat` e `historico.dat`, com `fwrite`, um registro por vez.
- Antes de gravar, o `prox` é zerado na cópia: endereço de memória não vale em outra execução.
- Ao carregar, a estrutura atual é liberada, cada nó é recriado com `malloc` e religado na ordem original, com `fread`.
- Se o arquivo não abrir, o programa mostra o erro e segue funcionando.

## Testes realizados

- Os Exemplos 1 a 6 do enunciado foram executados em sequência e as saídas conferem com as do enunciado, incluindo a pilha numerada, o histórico e a persistência (salvar, encerrar, executar de novo e carregar).
- Casos de borda testados: cancelar protocolo no início, no meio e no fim da fila e enfileirar de novo; atender com a fila vazia; as três opções quando não há equipamento disponível; equipamento em manutenção não é emprestado; devolução e manutenção de equipamento inexistente; entradas inválidas no menu e nos números; carregar sem arquivos, com arquivos vazios e com arquivo truncado; carregar com dados já na memória.
- Testes manuais sem erros/problemas relevantes.
- Teste de estresse realizado com Claude com 3000 equipamentos cadastrados fora de ordem, 1500 solicitações e cancelamentos, com salvar e recarregar: a ordem e os dados foram preservados.
- Verificação de memória com AddressSanitizer: nenhum erro e nenhum vazamento.

## Limitações conhecidas

- Fila, pilha e contador de protocolo não são salvos (o enunciado trata como melhoria). Depois de carregar, a pilha começa vazia e o protocolo recomeça do 1.
- A regra da mesma categoria é verificada no registro da solicitação, como pede o enunciado. Se a mesma pessoa registrar duas solicitações da mesma categoria antes de ser atendida, as duas podem ser atendidas.
- Texto maior que o vetor não corrompe a memória, mas o que sobrar é lido como o próximo campo.
- Números como `3.7` e `12abc` são lidos como `3` e `12`. Identificadores zero e negativos são aceitos.
- Arquivo `.dat` truncado: os registros completos são carregados e o resto é ignorado, sem aviso.
- Se só um dos arquivos de dados existir, o programa avisa o erro, mas carrega o que encontrou.
- A listagem não alinha as colunas: os campos são separados por ` | `.