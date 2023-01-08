#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define COLS 60
#define ROWS 30

void printa_tabuleiro() {
    printf("┌");
    for (int i = 0; i < COLS; i++) {
        printf("─");
    }
    printf("┐\n");

    for (int j = 0; j < ROWS; j++) {
        printf("|");
        for (int i = 0; i < COLS; i++) {
            printf("·");
        }
        printf("|\n");
    }

    printf("└");
    for (int i = 0; i < COLS; i++) {
        printf("─");
    }
    printf("┘\n");
}

void cria_nova_maca(int *xmaca, int *ymaca) {
    xmaca = rand() % COLS;
    ymaca = rand() % ROWS;
}

int main() {
    int quit = 0;
    while (!quit) {
        printa_tabuleiro();
    }

    return 0;
}