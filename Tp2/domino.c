#include "domino.h"
#include <stdlib.h>
#include <stdio.h>

bool LeituraDomino() {
    Lista *lista;
    Domino domino;
    int tam, teste = 0;
    while (scanf("%d", &tam) == 1) {
        if (tam == 0)
            return true;
        teste++;
        lista = DominoCria();
        
        if (!lista){
        return false;
        }

        int *vetorNumeros = calloc(7, sizeof(int));
        if (!vetorNumeros) {
            DominoDestroi(lista);
            return false;
        }
        for(int i = 0; i < tam; i++) {
            scanf("%d%d", &domino.ladoA, &domino.ladoB);
            DominoAdicionaPecaFinal(lista, domino);
            vetorNumeros[domino.ladoA]++;
            vetorNumeros[domino.ladoB]++;
        }
        int impares = 0;
        for(int i = 0; i < 7; i++) {
            if (vetorNumeros[i] % 2 != 0)
                impares++;
        }
        free(vetorNumeros);
        if (impares > 2) {
            DominoImprime(lista, false, teste);
            DominoDestroi(lista);
            continue;
        }
        DominoEhPossivelOrganizar(lista, tam, teste);
    }
    return true;
}

Lista *DominoCria() {
    Lista *lista = (Lista *)malloc(sizeof(Lista));
    if (lista == NULL)
        return NULL;
    lista->pPrimeiro = NULL;
    lista->pUltimo = NULL;
    return lista;
}

bool DominoEhPossivelOrganizar(Lista *listaLeitura, int tam, int teste) {
    Lista *lista = DominoCria();
    if (lista == NULL) 
        return false;
    bool *pecasUsadas = calloc(tam, sizeof(bool));
    if (!pecasUsadas) {
        DominoDestroi(lista);
        return false;
    }
    bool resultado = DominoEhPossivelOrganizarRec(listaLeitura, lista, pecasUsadas, tam, 0);
    Lista *listaCorreta = DominoCria();
    if (!listaCorreta) {
        free(pecasUsadas);
        DominoDestroi(lista);
        return false;
    }
    invertePilha(lista, listaCorreta);
    DominoImprime(listaCorreta, resultado, teste);
    DominoDestroi(listaLeitura);
    DominoDestroi(lista);
    DominoDestroi(listaCorreta);
    free(pecasUsadas);
    return resultado;
}

bool DominoEhPossivelOrganizarRec(Lista *listaLeitura, Lista *lista, bool *pecasUsadas, int tam, int camada) {
    if (camada == tam)
        return true;
    Celula *aux = listaLeitura->pPrimeiro;
    for (int i = 0; i < tam && aux != NULL; i++) {
        if (!pecasUsadas[i]) {
            Domino pecaAtual = aux->domino;
            if (listaEhVazia(lista)) {
                DominoAdicionaPecaInicio(lista, pecaAtual);
                pecasUsadas[i] = true;
                if (DominoEhPossivelOrganizarRec(listaLeitura, lista, pecasUsadas, tam, camada + 1))
                    return true;
                DominoRemoveInicio(lista);
                pecasUsadas[i] = false;
            } else {
                if (encaixa(lista->pPrimeiro->domino, pecaAtual)) {
                    DominoAdicionaPecaInicio(lista, pecaAtual);
                    pecasUsadas[i] = true;
                    if (DominoEhPossivelOrganizarRec(listaLeitura, lista, pecasUsadas, tam, camada + 1))
                        return true;
                    DominoRemoveInicio(lista);
                    pecasUsadas[i] = false;
                }
                Domino pecaAtualInvertida = {pecaAtual.ladoB, pecaAtual.ladoA};
                if (encaixa(lista->pPrimeiro->domino, pecaAtualInvertida)) {
                    DominoAdicionaPecaInicio(lista, pecaAtualInvertida);
                    pecasUsadas[i] = true;
                    if (DominoEhPossivelOrganizarRec(listaLeitura, lista, pecasUsadas, tam, camada + 1))
                        return true;
                    DominoRemoveInicio(lista);
                    pecasUsadas[i] = false;
                }
            }
        }
        aux = aux->prox;
    }
    return false;
}

bool encaixa(Domino domino1, Domino domino2) {
    return domino1.ladoB == domino2.ladoA;
}

bool DominoAdicionaPecaInicio(Lista *lista, Domino domino) {
    Celula *novaCelula = (Celula *)malloc(sizeof(Celula));
    if (novaCelula == NULL)
        return false;
    novaCelula->domino = domino;
    novaCelula->prox = lista->pPrimeiro;
    lista->pPrimeiro = novaCelula;
    if (lista->pUltimo == NULL)
        lista->pUltimo = novaCelula;
    return true;
}

bool DominoAdicionaPecaFinal(Lista *lista, Domino domino) {
    Celula *novaCelula = (Celula*)malloc(sizeof(Celula));
    if (novaCelula == NULL)
        return false;
    novaCelula->domino = domino;
    novaCelula->prox = NULL;
    if (lista->pUltimo)
        lista->pUltimo->prox = novaCelula;
    lista->pUltimo = novaCelula;
    if (lista->pPrimeiro == NULL)
        lista->pPrimeiro = novaCelula;
    return true;
}

bool DominoRemoveInicio(Lista *lista) {
    if (listaEhVazia(lista))
        return false;
    Celula *aux = lista->pPrimeiro;
    lista->pPrimeiro = aux->prox;
    if (lista->pPrimeiro == NULL)
        lista->pUltimo = NULL;
    free(aux);
    return true;
}

void DominoDestroi(Lista *lista) {
    while (!listaEhVazia(lista))
        DominoRemoveInicio(lista);
    free(lista);
}

void DominoImprime(Lista *lista, bool resul, int teste) {
    printf("Test %d:\n%s\n", teste, resul ? "YES" : "NO\n");
    if (resul)
        DominoImprimeLista(lista);
}

bool listaEhVazia(Lista *lista) {
    return lista->pPrimeiro == NULL;
}

void DominoImprimeLista(Lista *lista) {
    Celula *aux = lista->pPrimeiro;
    while (aux != NULL) {
        printf("%d%d", aux->domino.ladoA, aux->domino.ladoB);
        if (aux->prox != NULL)
            printf("|");
        aux = aux->prox;
    }
    printf("\n\n");
}

void invertePilha(Lista *lista, Lista *listaCorreta) {
    Celula *aux = lista->pPrimeiro;
    while (aux != NULL) {
        DominoAdicionaPecaInicio(listaCorreta, aux->domino);
        aux = aux->prox;
    }
}