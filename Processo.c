#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

typedef struct {
    char id_classe[255];
    int count;
} ClasseCount;

int converter_data_para_int(const char *data) {
    int ano, mes, dia;
    sscanf(data, "%d-%d-%d", &ano, &mes, &dia);
    return ano * 365 + mes * 30 + dia;
}

int lerDados(const char *nome_arquivo, Processo *X, int max_registros) {
    FILE *abre_arq = fopen(nome_arquivo, "r");
    if (!abre_arq) {
        printf("Erro ao abrir o arquivo\n");
        return -1;
    }

    char linha[1024];
    fgets(linha, sizeof(linha), abre_arq); // pula cabeçalho
    int i = 0;

    while (i < max_registros && fgets(linha, sizeof(linha), abre_arq)) {
        if (sscanf(linha, "%d,\"%[^\"]\",%[^,],{%[^}]},{%[^}]},%d",
                   &X[i].id, X[i].numero, X[i].data_ajuizamento, X[i].id_classe, X[i].id_assunto, &X[i].ano_eleicao) == 6 ||
            sscanf(linha, "%d,\"%[^\"]\",%[^,],\"{%[^}]}\",{%[^}]},%d",
                   &X[i].id, X[i].numero, X[i].data_ajuizamento, X[i].id_classe, X[i].id_assunto, &X[i].ano_eleicao) == 6 ||
            sscanf(linha, "%d,\"%[^\"]\",%[^,],{%[^}]},\"{%[^}]}\",%d",
                   &X[i].id, X[i].numero, X[i].data_ajuizamento, X[i].id_classe, X[i].id_assunto, &X[i].ano_eleicao) == 6) {
            i++;
        }
    }

    fclose(abre_arq);
    return i;
}

void imprimirDados(Processo *X, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("id: %d, numero: %s, data: %s, classe: %s, assunto: %s, ano: %d\n",
               X[i].id, X[i].numero, X[i].data_ajuizamento, X[i].id_classe, X[i].id_assunto, X[i].ano_eleicao);
    }
}

void insertion_sort(Processo *X, int n) {
    for (int i = 1; i < n; i++) {
        Processo chave = X[i];
        int j = i - 1;
        while (j >= 0 && X[j].id > chave.id) {
            X[j + 1] = X[j];
            j--;
        }
        X[j + 1] = chave;
    }

    FILE *f = fopen("id_ordenado.csv", "w");
    fprintf(f, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < n; i++)
        fprintf(f, "%d,\"%s\",%s,\"%s\",\"%s\",%d\n", X[i].id, X[i].numero, X[i].data_ajuizamento,
                X[i].id_classe, X[i].id_assunto, X[i].ano_eleicao);
    fclose(f);
}

void ordenar_por_data_ajuizamento(Processo *X, int n) {
    for (int i = 1; i < n; i++) {
        Processo chave = X[i];
        int j = i - 1;
        while (j >= 0 && converter_data_para_int(X[j].data_ajuizamento) < converter_data_para_int(chave.data_ajuizamento)) {
            X[j + 1] = X[j];
            j--;
        }
        X[j + 1] = chave;
    }

    FILE *f = fopen("data_ordenado.csv", "w");
    fprintf(f, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < n; i++)
        fprintf(f, "%d,\"%s\",%s,\"%s\",\"%s\",%d\n", X[i].id, X[i].numero, X[i].data_ajuizamento,
                X[i].id_classe, X[i].id_assunto, X[i].ano_eleicao);
    fclose(f);
}

int contaClasse(Processo *X, int n) {
    ClasseCount classes[1000];
    int total = 0;

    for (int i = 0; i < n; i++) {
        char tmp[255];
        strcpy(tmp, X[i].id_classe);
        char *token = strtok(tmp, ",");

        while (token) {
            int achou = 0;
            for (int j = 0; j < total; j++) {
                if (strcmp(classes[j].id_classe, token) == 0) {
                    classes[j].count++;
                    achou = 1;
                    break;
                }
            }
            if (!achou) {
                strcpy(classes[total].id_classe, token);
                classes[total].count = 1;
                total++;
            }
            token = strtok(NULL, ",");
        }
    }

    for (int i = 0; i < total; i++)
        printf("Classe: %s, Quantidade: %d\n", classes[i].id_classe, classes[i].count);

    return total;
}

int contaAssunto(Processo *X, int n) {
    char assuntos[1000][255];
    int total = 0;

    for (int i = 0; i < n; i++) {
        char tmp[255];
        strcpy(tmp, X[i].id_assunto);
        char *token = strtok(tmp, ",");

        while (token) {
            int achou = 0;
            for (int j = 0; j < total; j++) {
                if (strcmp(assuntos[j], token) == 0) {
                    achou = 1;
                    break;
                }
            }
            if (!achou) {
                strcpy(assuntos[total], token);
                total++;
            }
            token = strtok(NULL, ",");
        }
    }

    printf("Total de assuntos únicos: %d\n", total);
    return total;
}

int contemVirgula(Processo *X, int n) {
    int cont = 0;
    for (int i = 0; i < n; i++) {
        if (strchr(X[i].id_assunto, ',')) {
            cont++;
            printf("%d, %s, %s, %s, %s, %d\n", X[i].id, X[i].numero, X[i].data_ajuizamento,
                   X[i].id_classe, X[i].id_assunto, X[i].ano_eleicao);
        }
    }
    return cont;
}

void calculaDiasTramitacao(Processo *X, int n) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int dias_hoje = (tm_info->tm_year + 1900) * 365 + (tm_info->tm_mon + 1) * 30 + tm_info->tm_mday;

    printf("ID do Processo | Dias em Tramitação\n");
    for (int i = 0; i < n; i++) {
        int dias_proc = converter_data_para_int(X[i].data_ajuizamento);
        printf("id: %d, dias: %d\n", X[i].id, dias_hoje - dias_proc);
    }
}