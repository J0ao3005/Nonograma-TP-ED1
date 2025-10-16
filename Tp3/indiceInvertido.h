#ifndef INDICEINVERTIDO_H
#define INDICEINVERTIDO_H

#include "hash.h"

typedef struct {
    HashTable hash;
} IndiceInvertido;

IndiceInvertido* criaIndice();
void destroiIndice(IndiceInvertido *indice);
void insereDocumento(IndiceInvertido *indice, const char *documento, const char *palavra);
void consultaIndice(IndiceInvertido *indice, const char *consulta[], int numPalavras);
void imprimeIndice(IndiceInvertido *indice);

#endif