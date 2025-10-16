#include "domino.h"
#include <stdio.h>

int main() {
    if (!LeituraDomino()) {
        printf("Erro ao processar os dados!\n");
        return 1;
    }
    return 0;
}