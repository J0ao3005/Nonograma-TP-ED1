#ifndef NONOGRAM_H
#define NONOGRAM_H

typedef struct {
    int dimension;
    int **board;
    int **rowHints;
    int **colHints;
} Nonogram;

Nonogram *NonogramAllocate(int dimension, int **rowHints, int **colHints);
void NonogramFree(Nonogram *nonogram);
Nonogram *NonogramRead();
int NonogramPlay(Nonogram *nonogram);
void NonogramPrint(const Nonogram *nonogram);
int validateLine(int *line, int dimension, int *hints, int hintCount);
int validateBoard(Nonogram *nonogram);
int validatePartial(int *line, int dimension, int *hints, int hintCount, int pos);
int isPartialValid(Nonogram *nonogram, int row, int col);
int solveNonogram(Nonogram *nonogram, int row, int col, int *solutionCount);

#endif // NONOGRAM_H


