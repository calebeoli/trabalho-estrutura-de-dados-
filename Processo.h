#ifndef PROCESSO_H
#define PROCESSO_H

typedef struct {
    int id;
    char numero[255];
    char data_ajuizamento[255];
    char id_classe[255];
    char id_assunto[255];
    int ano_eleicao;
} Processo;

int lerDados(const char *nome_arquivo, Processo *X, int max_registros);
void imprimirDados(Processo *X, int qtd);
void insertion_sort(Processo *X, int total_registros);
void ordenar_por_data_ajuizamento(Processo *X, int total_registros);
int contaClasse(Processo *X, int total_registros);
int contaAssunto(Processo *X, int total_registros);
int contemVirgula(Processo *X, int total_registros);
void calculaDiasTramitacao(Processo *X, int total_registros);

#endif