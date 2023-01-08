#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

#define COLS 30
#define ROWS 15

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
    *xmaca = rand() % COLS;
    *ymaca = rand() % ROWS;

    for (int i = pos_cauda; i != pos_cabeca; i = (i + 1) % 1000) {
        if (xcobra[i] == *xmaca && ycobra[i] == *ymaca) {
            *xmaca = -1;
        }
    }

    if (xmaca >= 0) {
        printf("\e[%iB\e[%iC❤", *ymaca + 1, *xmaca + 1);
        printf("\e[%iF", *ymaca + 1);
    }
}

void verifica_comeu_maca(int *xmaca, int ymaca, int pos_cabeca, int *pos_cauda, int xcobra[], int ycobra[]) {
    if (xcobra[pos_cabeca] == *xmaca && ycobra[pos_cabeca] == ymaca) {
        *xmaca = -1;
        printf("\a");
    } else {
        *pos_cauda = (*pos_cauda + 1) % 1000;
    }
}

void atualiza_pos_cauda(int pos_cauda, int xcobra[], int ycobra[]) {
    printf("\e[%iB\e[%iC·", ycobra[pos_cauda] + 1, xcobra[pos_cauda] + 1);
    printf("\e[%iF", ycobra[pos_cauda] + 1);
}

int atualiza_pos_cabeca(int pos_cabeca, int xcobra[], int ycobra[], int xdir, int ydir) {
    int cabeca_temp = (pos_cabeca + 1) % 1000;
    xcobra[cabeca_temp] = (xcobra[pos_cabeca] + xdir + COLS) % COLS;
    ycobra[cabeca_temp] = (ycobra[pos_cabeca] + ydir + ROWS) % ROWS;
    return cabeca_temp;
}

int checa_game_over(int pos_cabeca, int pos_cauda, int xcobra[], int ycobra[]) {
    for (int i = pos_cauda; i != pos_cabeca; i = (i + 1) % 1000) {
        if (xcobra[i] == xcobra[pos_cabeca] && ycobra[i] == ycobra[pos_cabeca]) {
            return 1;
        }
    }
    return 0;
}

void desenha_cabeca(int pos_cabeca, int pos_cauda, int xcobra[], int ycobra[]) {
    printf("\e[%iB\e[%iC▓", ycobra[pos_cabeca] + 1, xcobra[pos_cabeca] + 1);
    printf("\e[%iF", ycobra[pos_cabeca] + 1);
    fflush(stdout);
}

void le_comandos(int *xdir, int *ydir, int *quit) {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    if (FD_ISSET(STDIN_FILENO, &fds)) {
        int tecla = getchar();
        
        if (tecla == 27 || tecla == 'q') {
            *quit = 1;
        } else if (tecla == 'w') {
            *xdir = 0;
            *ydir = -1;
        } else if (tecla == 'a') {
            *xdir = -1;
            *ydir = 0;
        } else if (tecla == 's') {
            *xdir = 0;
            *ydir = 1;
        } else if (tecla == 'd') {
            *xdir = 1;
            *ydir = 0;
        }
    }
}

void game_over() {
    printf("\e[%iB\e[%iC Game Over! ", ROWS / 2, COLS / 2 - 5);
    printf("\e[%iF", ROWS / 2);
    fflush(stdout);
}


int main() {
    printf("\e[?25l");

    // nerdolagem com modo canônico de leitura
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int xcobra[1000], ycobra[1000];
    int quit = 0;
    while (!quit) {
        printa_tabuleiro();
        printf("\e[%iA", ROWS + 2);

        // inicaliza valores relacionados as posições
        int pos_cabeca = 0, pos_cauda = 0;
        xcobra[pos_cabeca] = COLS / 2;
        ycobra[pos_cauda] = ROWS / 2;
        int fim_jogo = 0;
        int xdir = 1, ydir = 0;
        int xmaca = -1, ymaca;

        while (!quit && !fim_jogo) {

            if (xmaca < 0) {
                cria_nova_maca(&xmaca, &ymaca, pos_cabeca, pos_cauda, xcobra, ycobra);
            }
            
            atualiza_pos_cauda(pos_cauda, xcobra, ycobra);
            verifica_comeu_maca(&xmaca, ymaca, pos_cabeca, &pos_cauda, xcobra, ycobra);
            pos_cabeca = atualiza_pos_cabeca(pos_cabeca, xcobra, ycobra, xdir, ydir);
            fim_jogo = checa_game_over(pos_cabeca, pos_cauda, xcobra, ycobra);
            desenha_cabeca(pos_cabeca, pos_cauda, xcobra, ycobra);

            usleep(5 * 1000000 / 60);

            le_comandos(&xdir, &ydir, &quit);
        }

        if (!quit) {
            game_over();
            getchar();
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    system("clear");
    return 0;
}