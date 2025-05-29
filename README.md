# Análise de Dados de Processos da Justiça Eleitoral do Distrito Federal

Este projeto consiste na implementação em C de um algoritmo para leitura e manipulação de dados formatados de um arquivo texto (CSV) contendo informações sobre processos que tramitam na justiça eleitoral do Distrito Federal. O trabalho foi desenvolvido como parte da disciplina de Algoritmos e Programação Estruturada (APE) da Universidade Católica de Brasília (UCB).

## Descrição do Projeto

O objetivo principal é processar um arquivo CSV (`processo_043_202409032338.csv`) que contém dados de processos eleitorais. Cada linha do arquivo (exceto a primeira) representa um processo e inclui os seguintes atributos: `id`, `numero`, `data_ajuizamento`, `id_classe`, `id_assunto`, e `ano_eleicao`[cite: 2]. Os atributos são separados por vírgulas, e os atributos multivalorados (como `id_classe` e `id_assunto`) são delimitados por chaves `{}`[cite: 3, 8].

## Funcionalidades Implementadas

O programa realiza as seguintes operações nos dados dos processos:

1.  **Ordenação por ID**: Ordena o conjunto de dados em ordem crescente com base no atributo "id". O resultado é armazenado em um novo arquivo CSV, mantendo o cabeçalho[cite: 1, 8].
2.  **Ordenação por Data de Ajuizamento**: Ordena o conjunto de dados em ordem decrescente com base no atributo "data_ajuizamento". O resultado também é armazenado em um novo arquivo CSV, mantendo o cabeçalho[cite: 1, 8].
3.  **Contagem de Processos por Classe**: Conta quantos processos estão vinculados a um determinado `id_classe`[cite: 1].
4.  **Identificação de Assuntos**: Identifica quantos `id_assuntos` distintos existem na base de dados[cite: 1, 4].
5.  **Processos com Múltiplos Assuntos**: Lista todos os processos que estão vinculados a mais de um `id_assunto`[cite: 1].
6.  **Tempo de Tramitação**: Indica a quantos dias um processo está em tramitação na justiça, considerando a data de ajuizamento[cite: 1, 5].

## Estrutura do Código

O código-fonte está estruturado de forma modular em três arquivos, conforme as diretrizes da aula de Tipo Abstrato de Dados (TAD)[cite: 6, 10].

* **Tipo Abstrato de Dados (TAD) "Processo"**: Foi implementado um TAD `Processo` para representar cada registro do arquivo CSV, seguindo as diretrizes dadas em aula[cite: 5].
* **Organização dos Dados**: Os dados são organizados em vetores de `structs`[cite: 7].
* **Funções por Item**: Cada item das funcionalidades listadas acima é resolvido por uma função específica[cite: 9].

## Como Compilar e Executar

1.  **Pré-requisitos**:
    * Um compilador C (ex: GCC).
2.  **Compilação**:
    Navegue até o diretório raiz do projeto e compile o código usando o seu compilador C. Exemplo com GCC:
    ```bash
    gcc -o main main.c process.c utils.c # (Exemplo de nomes de arquivos, ajuste conforme necessário)
    ```
3.  **Execução**:
    Após a compilação, execute o programa:
    ```bash
    ./main
    ```

## Formato dos Dados de Entrada

O arquivo de entrada (`processo_043_202409032338.csv`) possui o seguinte formato:

```csv
"id", "numero", "data_ajuizamento", "id_classe", "id_assunto", "ano_eleicao"
638633058, "00000103020166070018", 2016-04-20 15:03:40.000, {12554}, {11778},0
405287812, "06000824620216070000", 2021-07-01 16:33:15.000, {12377}, {11778}, 2020
405277919, "00000238420156070011", 2015-05-18 16:49:33.000, {11541},{11778},0
