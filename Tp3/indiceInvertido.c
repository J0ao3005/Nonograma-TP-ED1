#include "indiceInvertido.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

IndiceInvertido* criaIndice() {
    IndiceInvertido *indice = (IndiceInvertido*)malloc(sizeof(IndiceInvertido));
    if (!indice) {
        printf("Erro ao alocar memória para o índice invertido.\n");
        exit(1);
    }
    inicializaHash(&indice->hash);
    return indice;
}

void destroiIndice(IndiceInvertido *indice) {
    free(indice);
}

void insereDocumento(IndiceInvertido *indice, const char *documento, const char *palavra) {
    insereHash(&indice->hash, palavra, documento);
}

void consultaIndice(IndiceInvertido *indice, const char *consulta[], int numPalavras) {
    if (numPalavras == 0) {
        printf("none\n");
        return;
    }
    
    EntradaHash *entradaBase = buscaHash(&indice->hash, consulta[0]);
    if (!entradaBase) {
        printf("none\n");
        return;
    }
    
    char documentosValidos[MAX_DOCUMENTOS][50];
    int numDocumentosValidos = 0;
    
    for (int i = 0; i < entradaBase->qtdDocumentos; i++) {
        int valido = 1;
        for (int j = 1; j < numPalavras; j++) {
            EntradaHash *entrada = buscaHash(&indice->hash, consulta[j]);
            if (!entrada) {
                valido = 0;
                break;
            }
            int encontrado = 0;
            for (int k = 0; k < entrada->qtdDocumentos; k++) {
                if (strcmp(entrada->documentos[k], entradaBase->documentos[i]) == 0) {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                valido = 0;
                break;
            }
        }
        if (valido) {
            strcpy(documentosValidos[numDocumentosValidos++], entradaBase->documentos[i]);
        }
    }
    
    if (numDocumentosValidos == 0) {
        printf("none\n");
        return;
    }
    
    for (int i = 0; i < numDocumentosValidos - 1; i++) {
        for (int j = i + 1; j < numDocumentosValidos; j++) {
            if (strcmp(documentosValidos[i], documentosValidos[j]) > 0) {
                char temp[50];
                strcpy(temp, documentosValidos[i]);
                strcpy(documentosValidos[i], documentosValidos[j]);
                strcpy(documentosValidos[j], temp);
            }
        }
    }
    
    for (int i = 0; i < numDocumentosValidos; i++) {
        printf("%s\n", documentosValidos[i]);
    }
}

void imprimeIndice(IndiceInvertido *indice) {
    imprimeHash(&indice->hash);
}
