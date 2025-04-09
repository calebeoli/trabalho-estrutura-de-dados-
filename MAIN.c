#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

int main() {
    int max = 20000;
    Processo *processos = malloc(max * sizeof(Processo));

    int qtd = lerDados("A.csv", processos, max);
    if (qtd == -1) {
        free(processos);
        return 1;
    }

    contaClasse(processos, qtd);
    contaAssunto(processos, qtd);
    contemVirgula(processos, qtd);

    insertion_sort(processos, qtd);
    imprimirDados(processos, 10);

    ordenar_por_data_ajuizamento(processos, qtd);
    calculaDiasTramitacao(processos, qtd);

    free(processos);
    return 0;
}
