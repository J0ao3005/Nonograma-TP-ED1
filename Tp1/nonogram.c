#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nonogram.h"


Nonogram *NonogramAllocate(int dimension, int **rowHints, int **colHints) {
    Nonogram *nonogram = (Nonogram *)malloc(sizeof(Nonogram));
    if (!nonogram) {
        perror("Erro ao alocar memoria para o Nonogram");
        return NULL;
    }
    nonogram->dimension = dimension;

    nonogram->board = (int **)malloc(dimension * sizeof(int *));
    if (!nonogram->board) {
        free(nonogram);
        return NULL;
    }

    for (int i = 0; i < dimension; i++) {
        nonogram->board[i] = (int *)calloc(dimension, sizeof(int));
        if (!nonogram->board[i]) {
            while (--i >= 0) free(nonogram->board[i]); // Se a alocação falhar em uma linha, desalocamos as já alocadas até o momento
            free(nonogram->board);
            free(nonogram);
            return NULL;
        }
    }

    nonogram->rowHints = rowHints;
    nonogram->colHints = colHints;

    return nonogram;
}

void NonogramFree(Nonogram *nonogram) {
    if (!nonogram) return;

    for (int i = 0; i < nonogram->dimension; i++) {
        free(nonogram->board[i]);
    }
    free(nonogram->board);

    for (int i = 0; i < nonogram->dimension; i++) {
        free(nonogram->rowHints[i]);
        free(nonogram->colHints[i]);
    }
    free(nonogram->rowHints);
    free(nonogram->colHints);
    free(nonogram);
}


// Aloca duas matrizes (para linhas e colunas) de acordo com a quantidade necessaria de dicas
Nonogram *NonogramRead() {
    int dimension;
    scanf("%d", &dimension);

    int **rowHints = (int **)malloc(dimension * sizeof(int *));
    int **colHints = (int **)malloc(dimension * sizeof(int *));



    for (int i = 0; i < dimension; i++) {
        int count;
        scanf("%d", &count);
        colHints[i] = (int *)malloc((count + 1) * sizeof(int));
        colHints[i][0] = count;
        for (int j = 1; j <= count; j++) {
            scanf("%d", &colHints[i][j]);
        }
    }

    for (int i = 0; i < dimension; i++) {
        int count;
        scanf("%d", &count);
        rowHints[i] = (int *)malloc((count + 1) * sizeof(int));
        rowHints[i][0] = count;
        for (int j = 1; j <= count; j++) {
            scanf("%d", &rowHints[i][j]);
        }
    }

    return NonogramAllocate(dimension, rowHints, colHints);
}

int validateLine(int *line, int dimension, int *hints, int hintCount) {

    /*
    groupCont = contador de grupos encontrados até a posição atual da coluna
    currentGroup = tamanho do grupo no momento
    groups[dimension] = armazena o tamanho dos grupos encontrados
    */

    int groupCount = 0, currentGroup = 0;
    int groups[dimension];

    for (int i = 0; i < dimension; i++) {
        if (line[i] == 1) {
            currentGroup++;
        } else if (currentGroup > 0) {
            groups[groupCount++] = currentGroup; // Registra o grupo anterior antes de resetar
            currentGroup = 0;
        }
    }


    if (currentGroup > 0) {
        groups[groupCount++] = currentGroup;
    }

    if (groupCount != hintCount) return 0; // Comparando o número de grupos encontrados com o número de grupos na struct (passsado por referência)
    for (int i = 0; i < groupCount; i++) {
        if (groups[i] != hints[i]) return 0; // Comprando os grupos com o os grupos de tamanho já especificados na struct (passado por referência)
    }

    return 1;
}

int validateBoard(Nonogram *nonogram) {
    int dimension = nonogram->dimension;

    //Verificando linha por linha para ver se batem com as dicas
    for (int i = 0; i < dimension; i++) {
        if (!validateLine(nonogram->board[i], dimension, nonogram->rowHints[i] + 1, nonogram->rowHints[i][0])) {
            return 0;
        }
    }

    //Verificando coluna por coluna
    for (int j = 0; j < dimension; j++) {
        int col[dimension];
        for (int i = 0; i < dimension; i++) {
            col[i] = nonogram->board[i][j];
        }
        if (!validateLine(col, dimension, nonogram->colHints[j] + 1, nonogram->colHints[j][0])) {
            return 0;
        }
    }
    return 1;
}


//Valida as celulas já preenchidas ATÉ UMA POSIÇÃO ESPECIFICA
int validatePartial(int *line, int dimension, int *hints, int hintCount, int pos) {
    
    /*
    groupCont = grupos encontrados até a posição atual da coluna (pos)
    currentGroup = tamanho do grupo no momento
    groups[dimension] = armazena o tamanho dos grupos encontrados
    */

    int groupCount = 0, currentGroup = 0;
    int groups[dimension];

    for (int i = 0; i <= pos; i++) {
        if (line[i] == 1) {
            currentGroup++;
        } else if (currentGroup > 0) {
            groups[groupCount++] = currentGroup; // Registra o grupo anterior antes de resetar
            currentGroup = 0;
        }
    }

    /*Se a linha termina com 1, não será encontrado um 0 para ativar o registro(groups[groupCount]).
    É necessario uma verificação extra fora do loop para armazenar esse grupo final.*/

    if (currentGroup > 0) {
        groups[groupCount++] = currentGroup;
    }

    /*
    Se o indice do grupo ultrapasssa o número de dicas disponivel ou o se o tamanho
    do grupo for maior que as dicas, retornamos inválido
    */

    for (int i = 0; i < groupCount; i++) {
        if (i >= hintCount || groups[i] > hints[i]) return 0;
    }

    return 1;
}

// Valida as linhas e colunas e retorna verdadeiro (1) apenas se AMBAS estiverem certas
int isPartialValid(Nonogram *nonogram, int row, int col) {
    int dimension = nonogram->dimension;

    if (!validatePartial(nonogram->board[row], dimension, nonogram->rowHints[row] + 1, nonogram->rowHints[row][0], col)) {
        return 0;
    }

    int colData[dimension];
    for (int i = 0; i <= row; i++) {
        colData[i] = nonogram->board[i][col];
    }
    if (!validatePartial(colData, dimension, nonogram->colHints[col] + 1, nonogram->colHints[col][0], row)) {
        return 0;
    }

    return 1;
}


//Função responsavel pelo backtracking (valida o tabuleiro e as combinações de preenchimento)
int solveNonogram(Nonogram *nonogram, int row, int col, int *solutionCount) {

    //Verificando se chegou ao final do tabuleiro
    if (row == nonogram->dimension) {
        if (validateBoard(nonogram)) {
            printf("SOLUTION %d:\n", ++(*solutionCount));
            NonogramPrint(nonogram);
        }
        return 0;
    }

    /*
    Se esiver na última coluna da linha atual:
     - Avança para a proxima linhha
     - Volta para a primeira coluna
     
    Se não:
     - Fica na messma linha
     - Avança para a próxima coluna
     */

    int nextRow = (col == nonogram->dimension - 1) ? row + 1 : row;
    int nextCol = (col == nonogram->dimension - 1) ? 0 : col + 1;

    nonogram->board[row][col] = 1;
    if (isPartialValid(nonogram, row, col)) {
        solveNonogram(nonogram, nextRow, nextCol, solutionCount);
    }

    nonogram->board[row][col] = 0;
    if (isPartialValid(nonogram, row, col)) {
        solveNonogram(nonogram, nextRow, nextCol, solutionCount);
    }

    return 0;
}

int NonogramPlay(Nonogram *nonogram) {
    int solutionCount = 0;

    solveNonogram(nonogram, 0, 0, &solutionCount); // Passando a primeira posição do tabuleiro e a quantdidae de soluções como referência

    if (solutionCount == 0) {
        printf("No solution was found!\n");
    }

    return solutionCount;
}

void NonogramPrint(const Nonogram *nonogram) {
    for (int i = 0; i < nonogram->dimension; i++) {
        for (int j = 0; j < nonogram->dimension; j++) {
            printf("%c ", nonogram->board[i][j] ? '*' : '.'); // Se positivo (1) preenche com * , caso negativo (0) preenche com .
        }
        printf("\n");
    }
}
