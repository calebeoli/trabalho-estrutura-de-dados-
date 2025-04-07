#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char numero[255];
    char data_ajuizamento[255];
    char id_classe[255];
    char id_assunto[255];
    int ano_eleicao;
} Dados;

typedef struct {
    char id_classe[255];
    int count;
} ClasseCount;

int lerDados(const char *nome_arquivo, Dados *X, int max_registros); // Função para ler os dados do arquivo
void imprimirDados(Dados *X, int qtd); // Função para imprimir os dados
int comparar_por_id(const void *a, const void *b); // Função para comparar por ID
int converter_data_para_int(const char *data); // Função para converter data para inteiro
int comparar_por_data_ajuizamento(const void *a, const void *b); // Função para comparar por data
int contaClasse(Dados *X,int total_registros); // função contar quantos processos estão vinculados a um determinado “id_classe”;
int contaAssunto(Dados *X,int total_registros); // conta quantos assuntos unicos existem no arquivo.


int main() {
    int max_registros = 18392;
    Dados *X = malloc(max_registros * sizeof(Dados));

    int registros_lidos = lerDados("../A.csv", X, max_registros);
    if (registros_lidos == -1) {
        free(X);
        return 1;
    }

    // Ordena os dados pelo campo "id" usando qsort
    //qsort(X, registros_lidos, sizeof(Dados), comparar_por_id);

    // Ordena os dados pela data de ajuizamento em ordem decrescente
    //qsort(X, registros_lidos, sizeof(Dados), comparar_por_data_ajuizamento);

    //contaClasse(X, registros_lidos);

    int assuntos = contaAssunto(X, registros_lidos);
    printf("Total de assuntos únicos: %d\n", assuntos);

    //imprimirDados(X,242);

    free(X);
    return 0;
}

int lerDados(const char *nome_arquivo, Dados *X, int max_registros) {
    FILE *abre_arq = fopen(nome_arquivo, "r");
    if (abre_arq == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return -1;
    }

    char linha[1024];
    fgets(linha, sizeof(linha), abre_arq); // Lê o cabeçalho

    int i = 0;
    while (i < max_registros && fgets(linha, sizeof(linha), abre_arq)) {
        // Tenta ler a linha com a formatação principal
        if (sscanf(linha, "%d,\"%[^\"]\",%[^,],{%[^}]},{%[^}]},%d",
                   &X[i].id, X[i].numero, X[i].data_ajuizamento, X[i].id_classe, X[i].id_assunto, &X[i].ano_eleicao) == 6) {
            // Leitura bem-sucedida com a formatação principal
        } else {
            // Tenta ler com a formatação alternativa
            if (sscanf(linha, "%d,\"%[^\"]\",%[^,],\"{%[^}]}\",\"%[^\"]\",%d",
                       &X[i].id, X[i].numero, X[i].data_ajuizamento, X[i].id_classe, X[i].id_assunto, &X[i].ano_eleicao) == 6) {
                // Leitura bem-sucedida com a formatação alternativa
            }
        }
        i++;
    }

    fclose(abre_arq);
    return i; // Retorna o número de registros lidos
}

void imprimirDados(Dados *X, int qtd) {
    printf("Os %d primeiros dados lidos são:\n", qtd);
    for (int j = 0; j < qtd; j++) {
        printf("Lido: %d, %s, %s, %s, %s, %d\n",
               X[j].id, X[j].numero, X[j].data_ajuizamento, X[j].id_classe, X[j].id_assunto, X[j].ano_eleicao);
    }
}

int comparar_por_id(const void *a, const void *b) {
    Dados *dadoA = (Dados *)a;
    Dados *dadoB = (Dados *)b;
    return dadoA->id - dadoB->id;
}

// Converte data para um número inteiro
int converter_data_para_int(const char *data) {
    int ano, mes, dia;
    sscanf(data, "%d-%d-%d", &ano, &mes, &dia);
    return ano * 10000 + mes * 100 + dia; // Formato YYYYMMDD
}

// Compara para ordenar em ordem decrescente pela data de ajuizamento
int comparar_por_data_ajuizamento(const void *a, const void *b) {



    const Dados *dadoA = (const Dados *)a;
    const Dados *dadoB = (const Dados *)b;

    int dataA = converter_data_para_int(dadoA->data_ajuizamento);
    int dataB = converter_data_para_int(dadoB->data_ajuizamento);

    return dataB - dataA; // Ordem decrescente
}

int contaClasse(Dados *X, int total_registros) {
    ClasseCount classes[1000]; // Array para armazenar IDs de classe e suas contagens
    int total_classes = 0;

    for (int i = 0; i < total_registros; i++) {
        char id_classe_copy[255];
        strcpy(id_classe_copy, X[i].id_classe); // Faz uma cópia para não modificar o original

        char *token = strtok(id_classe_copy, ",");
        while (token != NULL) {
            // Verifica se o ID de classe já está no array
            int found = 0;
            for (int j = 0; j < total_classes; j++) {
                if (strcmp(classes[j].id_classe, token) == 0) {
                    classes[j].count++;
                    found = 1;
                    break;
                }
            }

            // Se o ID de classe não foi encontrado, adiciona ao array
            if (!found) {
                strcpy(classes[total_classes].id_classe, token);
                classes[total_classes].count = 1;
                total_classes++;
            }

            token = strtok(NULL, ","); // Próximo token
        }
    }

    // Imprime os resultados
    printf("Contagem de processos por ID de classe:\n");
    for (int i = 0; i < total_classes; i++) {
        printf("ID de classe: %s, Processos vinculados: %d\n", classes[i].id_classe, classes[i].count);
    }

    return total_classes; // Retorna o número total de IDs de classe únicos
}

int contaAssunto(Dados *X, int total_registros) {
    char assuntos_unicos[1000][255]; // Array para armazenar IDs de assunto únicos
    int total_assuntos = 0;

    for (int i = 0; i < total_registros; i++) {
        char id_assunto_copy[255];
        strcpy(id_assunto_copy, X[i].id_assunto); // Faz uma cópia para não modificar o original

        char *token = strtok(id_assunto_copy, ",");
        while (token != NULL) {
            // Verifica se o ID de assunto já está no array de únicos
            int found = 0;
            for (int j = 0; j < total_assuntos; j++) {
                if (strcmp(assuntos_unicos[j], token) == 0) {
                    found = 1;
                    break;
                }
            }

            // Se o ID de assunto não foi encontrado, adiciona ao array
            if (!found) {
                strcpy(assuntos_unicos[total_assuntos], token);
                total_assuntos++;
            }

            token = strtok(NULL, ","); // Próximo token
        }
    }

    return total_assuntos; // Retorna o número total de IDs de assunto únicos
}



