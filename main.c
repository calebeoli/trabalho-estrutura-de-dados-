#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
void insertion_sort(Dados *X, int total_registros); // Função para comparar por ID
int converter_data_para_int(const char *data) ; // Função para converter data para inteiro
void ordenar_por_data_ajuizamento(Dados *X, int total_registros) ; // Função para comparar por data
int contaClasse(Dados *X,int total_registros); // função contar quantos processos estão vinculados a um determinado “id_classe”;
int contaAssunto(Dados *X,int total_registros); // conta quantos assuntos unicos existem no arquivo.
int contemVirgula(Dados *X,int total_registros); // Função para verificar quantos id_assuntos tem mais de um assunto.
void calculaDiasTramitacao(Dados *X, int total_registros); // Função para calcular os dias de tramitação


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

   //contaAssunto(X, registros_lidos);


    //int linhas_com_virgula = contemVirgula(X, registros_lidos);
    //printf("Total de linhas com virgula no id_assunto: %d\n", linhas_com_virgula);

    //insertion_sort(X, registros_lidos);
    //imprimirDados(X,10);

    //ordenar_por_data_ajuizamento(X, registros_lidos);

    //calculaDiasTramitacao(X, registros_lidos);

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
            if (sscanf(linha, "%d,\"%[^\"]\",%[^,],\"{%[^}]}\",{%[^}]},%d",
                       &X[i].id, X[i].numero, X[i].data_ajuizamento, X[i].id_classe, X[i].id_assunto, &X[i].ano_eleicao) == 6) {
                // Leitura bem-sucedida com a formatação alternativa
            } else {
                if (sscanf(linha, "%d,\"%[^\"]\",%[^,],{%[^}]},\"{%[^}]}\",%d",
                    &X[i].id, X[i].numero, X[i].data_ajuizamento, X[i].id_classe, X[i].id_assunto, &X[i].ano_eleicao) == 6) {
                    
                    
                }
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


void insertion_sort(Dados *X, int total_registros) {
    for (int i = 1; i < total_registros; i++) {
        Dados chave = X[i];
        int j = i - 1;

        // Move elementos maiores que a chave para frente
        while (j >= 0 && X[j].id > chave.id) {
            X[j + 1] = X[j];
            j--;
        }
        X[j + 1] = chave;
    }
        FILE *arquivo = fopen("../id_ordenado.csv", "w");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo para escrita\n");
            return;
        }
        fprintf(arquivo, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
        for (int i = 0; i < total_registros; i++) {
            fprintf(arquivo, "%d,\"%s\",%s,\"%s\",\"%s\",%d\n",
                    X[i].id, X[i].numero, X[i].data_ajuizamento, X[i].id_classe, X[i].id_assunto, X[i].ano_eleicao);
        }
        fclose(arquivo);
}

int converter_data_para_int(const char *data) {
    int ano, mes, dia;
    sscanf(data, "%d-%d-%d", &ano, &mes, &dia); // Extrai ano, mês e dia da string
    return (ano * 365) + (mes * 30) + dia; // Converte a data para um número total de dias
}

// Compara para ordenar em ordem decrescente pela data de ajuizamento
void ordenar_por_data_ajuizamento(Dados *X, int total_registros) {
    for (int i = 1; i < total_registros; i++) {
        Dados chave = X[i];
        int j = i - 1;

        // Converte a data de ajuizamento para inteiro para comparação
        int data_chave = converter_data_para_int(chave.data_ajuizamento);

        // Move os elementos que têm data menor para frente
        while (j >= 0 && converter_data_para_int(X[j].data_ajuizamento) < data_chave) {
            X[j + 1] = X[j];
            j--;
        }
        X[j + 1] = chave;
    }
    FILE *arquivo = fopen("../data_ordenado.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita\n");
        return;
    }
    fprintf(arquivo, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < total_registros; i++) {
        fprintf(arquivo, "%d,\"%s\",%s,\"%s\",\"%s\",%d\n",
                X[i].id, X[i].numero, X[i].data_ajuizamento, X[i].id_classe, X[i].id_assunto, X[i].ano_eleicao);
    }
    fclose(arquivo);
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

    return    printf("Total de assuntos unicos: %d\n", total_assuntos); // Retorna o número total de IDs de assunto únicos
}

int contemVirgula(Dados *X, int total_registros) {
    int num_linhas = 0;

    for (int i = 0; i < total_registros; i++) { // Corrigido: i < total_registros
        if (strchr(X[i].id_assunto, ',') != NULL) { // Verifica se há uma vírgula no campo id_assunto
            
            printf("%d, %s, %s, %s, %s, %d\n",
                   X[i].id, X[i].numero, X[i].data_ajuizamento, X[i].id_classe, X[i].id_assunto, X[i].ano_eleicao);
            num_linhas++;
        }
    }

    return num_linhas; // Retorna o número de processos com mais de um assunto
}



void calculaDiasTramitacao(Dados *X, int total_registros) {
    // Obtém a data atual
    time_t t = time(NULL);
    struct tm *data_atual = localtime(&t);

    // Converte a data atual para o formato de dias
    int dias_atuais = (data_atual->tm_year + 1900) * 365 + (data_atual->tm_mon + 1) * 30 + data_atual->tm_mday;

    printf("ID do Processo | Dias em Tramitação\n");


    for (int i = 0; i < total_registros; i++) {
        // Converte a data de ajuizamento para o formato de dias
        int dias_ajuizamento = converter_data_para_int(X[i].data_ajuizamento);

        // Calcula a diferença em dias
        int dias_tramitacao = dias_atuais - dias_ajuizamento;

        // Exibe o ID do processo e os dias em tramitação
        printf("id: %d, dias: %d \n", X[i].id, dias_tramitacao);
    }
}


