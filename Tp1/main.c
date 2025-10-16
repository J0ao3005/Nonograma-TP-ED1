#include <stdio.h>
#include "nonogram.h"

int main() {
    Nonogram *nonogram = NonogramRead();
    if (!nonogram) {
        return 1;
    }
    
    int solutions = NonogramPlay(nonogram);

    if(solutions > 0){
        printf("Total of solutions: %d\n", solutions);
    }

    NonogramFree(nonogram);
    return 0;
}