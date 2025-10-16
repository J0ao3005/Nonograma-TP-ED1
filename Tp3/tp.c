#include "indiceInvertido.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int N;
    scanf("%d", &N);

    IndiceInvertido *indice = criaIndice();

    for (int i = 0; i < N; i++) {
        char documento[50];
        char palavra[MAX_PALAVRA];

        scanf("%s", documento);
        while (scanf("%s", palavra) == 1) {
            insereDocumento(indice, documento, palavra);
            if (getchar() == '\n') break;
        }
    }

    char opcao;
    scanf(" %c", &opcao);

    if (opcao == 'B') {
        char consulta[MAX_PALAVRA];
        char *termos[100];
        int numPalavras = 0;
        while (scanf("%s", consulta) == 1) {
            termos[numPalavras] = strdup(consulta);
            numPalavras++;
            if (getchar() == '\n') break;
        }
        consultaIndice(indice, (const char **)termos, numPalavras);
        for (int i = 0; i < numPalavras; i++) {
            free(termos[i]);
        }
    } else if (opcao == 'I') {
        imprimeIndice(indice);
    }

    destroiIndice(indice);
    return 0;
}