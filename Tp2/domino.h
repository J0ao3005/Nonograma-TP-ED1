#ifndef DOMINO_H
#define DOMINO_H

#include <stdbool.h>
//Representacao da peca(Domino)
typedef struct {
    int ladoA;
    int ladoB;
} Domino;

//Celula
typedef struct cell {
    struct cell *prox;
    Domino domino;
} Celula;

//Sentinela
typedef struct {
    Celula *pPrimeiro, *pUltimo;
} Lista;

//Leitura e inicializacao
bool LeituraDomino(); //Realiza a leitura da entrada
Lista *DominoCria(); //Inicializa a cabeça

//Verificacoes e impressao
bool listaEhVazia(Lista *); //Retorna se a lista eh vazia
void DominoImprime(Lista *, bool, int); //Imprime  a lista
void DominoImprimeLista(Lista *);

//Resolucao do Problema
bool DominoEhPossivelOrganizar(Lista *, int, int);
bool DominoEhPossivelOrganizarRec(Lista *, Lista *, bool *, int, int); 
bool encaixa(Domino, Domino);

//Manipulacoes da Lista
bool DominoAdicionaPecaInicio(Lista *, Domino); //Adiciona no inicio
bool DominoAdicionaPecaFinal(Lista *, Domino); // Adiciona no Final
bool DominoRemoveInicio(Lista *); //Remove no inicio  
bool removeFinal(Lista *); //Remove final
void DominoDestroi(Lista *); //Desaloca totalmente
void invertePilha(Lista *, Lista *); //inverte a pilha

#endif //DOMINO_H