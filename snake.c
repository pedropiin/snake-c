#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
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

void cria_nova_maca(int *xmaca, int *ymaca, int pos_cabeca, int pos_cauda, int xcobra[], int ycobra[]) {
    xmaca = rand() % COLS;
    ymaca = rand() % ROWS;

    for (int i = pos_cauda; i != pos_cabeca; i = (i + 1) % 1000) {
        if (xcobra[i] == *xmaca && ycobra[i] == ymaca) {
            *xmaca = -1;
        }
    }

    if (xmaca >= 0) {
        printf("\e[%iB\e[%iC❤", ymaca + 1, xmaca + 1);
        printf("\e[%iF", ymaca + 1);
    }
}

void verifica_comeu_maca(int *xmaca, int ymaca, int pos_cabeca, int *pos_cauda, int xcobra[], int ycobra[]) {
    if (xcobra[pos_cabeca] == xmaca && ycobra[pos_cabeca] == ymaca) {
        *xmaca = -1;
        printf("\a");
    } else {
        *pos_cauda = (*pos_cauda + 1) % 1000;
    }
}

int main() {
    cursor(0);

    // nerdolagem com modo canônico de leitura
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, %oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int xcobra[1000], ycobra[1000];
    int quit = 0;
    while (!quit) {

        printa_tabuleiro();

        // inicaliza valores relacionados as posições
        int pos_cabeca = 0, pos_cauda = 0;
        xcobra[pos_cabeca] = COLS / 2;
        ycobra[pos_cauda] = ROWS / 2;
        int fim_jogo = 0;
        int xdir = 1, ydir = 0;
        int xmaca = -1, ymaca;

        if (xmaca < 0) {
            cria_nova_maca(&xmaca, &ymaca, pos_cabeca, pos_cauda, xcobra, ycobra);
        }

        /*
        // Clear snake tail
        printf("\e[%iB\e[%iC·", y[tail] + 1, x[tail] + 1);
        printf("\e[%iF", y[tail] + 1);
        */

        verifica_comeu_maca(&xmaca, ymaca, pos_cabeca, &pos_cauda, xcobra, ycobra);

        
    }

    return 0;
}