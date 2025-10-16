#ifndef HASH_H
#define HASH_H

#define MAX_VOCAB 1000
#define MAX_PALAVRA 20
#define MAX_DOCUMENTOS 100

typedef struct {
    char palavra[MAX_PALAVRA];
    char documentos[MAX_DOCUMENTOS][50];
    int qtdDocumentos;
} EntradaHash;

typedef struct {
    EntradaHash tabela[MAX_VOCAB];
    int tamanho;
    char ordemInsercao[MAX_VOCAB][MAX_PALAVRA];
    int qtdPalavras;
} HashTable;

void inicializaHash(HashTable *hash);
int insereHash(HashTable *hash, const char *chave, const char *documento);
EntradaHash* buscaHash(HashTable *hash, const char *chave);
void imprimeHash(HashTable *hash);

#endif
