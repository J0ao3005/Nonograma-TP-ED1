#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int h(const char *chave, int m) {
    float p[] = {0.8326030060567271, 0.3224428884580177, 
                 0.6964223353369197, 0.1966079596929834, 
                 0.8949283476433433, 0.4587297824155836, 
                 0.5100785238948532, 0.05356055934904358, 
                 0.9157270141062215, 0.7278472432221632};
    int tamP = 10;
    unsigned int soma = 0;
    for (int i = 0; i < strlen(chave); i++) {
        soma += (unsigned int) chave[i] * p[i % tamP];
    }
    return soma % m;
}

void inicializaHash(HashTable *hash) {
    hash->tamanho = 0;
    hash->qtdPalavras = 0;
    for (int i = 0; i < MAX_VOCAB; i++) {
        hash->tabela[i].palavra[0] = '\0';
        hash->tabela[i].qtdDocumentos = 0;
    }
}

// Insere uma palavra e seu documento na tabela hash
int insereHash(HashTable *hash, const char *chave, const char *documento) {
    // Verifica se a palavra já foi inserida na ordem de inserção
    int encontrada = 0;
    for (int i = 0; i < hash->qtdPalavras; i++) {
        if (strcmp(hash->ordemInsercao[i], chave) == 0) {
            encontrada = 1;
            break;
        }
    }
    if (!encontrada) {
        // Adiciona a palavra à ordem de inserção
        strcpy(hash->ordemInsercao[hash->qtdPalavras++], chave);
    }

    // Obtém o índice da palavra
    unsigned int indice = h((char *)chave, MAX_VOCAB);
    
    // Insere o documento na tabela hash
    for (int i = 0; i < MAX_VOCAB; i++) {
        int pos = (indice + i) % MAX_VOCAB;

        // Se a posição estiver vazia, insira a palavra e inicialize a quantidade de documentos
        if (hash->tabela[pos].palavra[0] == '\0') {
            strcpy(hash->tabela[pos].palavra, chave);
            hash->tabela[pos].qtdDocumentos = 0;
        }

        // Quando encontrar a palavra, insira o documento se não estiver presente
        if (strcmp(hash->tabela[pos].palavra, chave) == 0) {
            // Verifica se o documento já foi inserido
            for (int j = 0; j < hash->tabela[pos].qtdDocumentos; j++) {
                if (strcmp(hash->tabela[pos].documentos[j], documento) == 0) {
                    return 0;
                }
            }

            // Adiciona o documento
            strcpy(hash->tabela[pos].documentos[hash->tabela[pos].qtdDocumentos++], documento);
            return 1;
        }
    }

    return 0;
}

// Busca uma palavra na tabela hash
EntradaHash* buscaHash(HashTable *hash, const char *chave) {
    unsigned int indice = h(chave, MAX_VOCAB);
    for (int i = 0; i < MAX_VOCAB; i++) {
        int pos = (indice + i) % MAX_VOCAB;
        if (hash->tabela[pos].palavra[0] == '\0') {
            return NULL;
        }
        if (strcmp(hash->tabela[pos].palavra, chave) == 0) {
            return &hash->tabela[pos];
        }
    }
    return NULL;
}

// Imprime as palavras e documentos da tabela hash
void imprimeHash(HashTable *hash) {
    for (int i = 0; i < MAX_VOCAB; i++) {
        if (hash->tabela[i].palavra[0] != '\0') {
            printf("%s -", hash->tabela[i].palavra);
            for (int j = 0; j < hash->tabela[i].qtdDocumentos; j++) {
                printf(" %s", hash->tabela[i].documentos[j]);
            }
            printf("\n");
        }
    }
}