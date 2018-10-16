#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#define O 79
#define X 88
#define WHITECELL 177
#define BLACKCELL 178

static unsigned char ***board; /* [z][y][x] yes those are three asterisks, yes I am insane */

void initializeboard(const unsigned char s) {
    unsigned char z, y, x;
    board = malloc(s * sizeof(unsigned char**)); /* allocate space for variable-sized board */
    for (z = 0; z < s; z++) {
        board[z] = malloc(s * sizeof(unsigned char*));
        for (y = 0; y < s; y++) {
            board[z][y] = malloc(s * sizeof(unsigned char));
            for (x = 0; x < s; x++) { /* draw squares */
                if ((z + y + x) % 2 == 0) {
                    board[z][y][x] = WHITECELL;
                }
                else {
                    board[z][y][x] = BLACKCELL;
                }
            }
        }
    }
}

void drawboard(const unsigned char s) {
    unsigned char z, y, x;
    unsigned char i;
    for (z = 0; z < s; z++) {
        for (y = 0; y < s; y++) {
            for (i = 0; i < (z * s); i++) { /* draw offset spaces for 3D */
                printf(" ");
            }
            for (x = 0; x < s; x++) { /* draw boxes */
                printf("%c", board[z][y][x]);
            }
            printf("\n");
        }
    }   
}

void makemove(const char t, const unsigned char s) {
    int z, y, x;
    move:
    printf("%c: ", t);
    scanf("%d%*c%d%*c%d", &z, &y, &x);
    if (z >= s || y >= s || x >= s) {
        printf("out of bounds\n");
        goto move;
    }
    else if (board[z][y][x] != WHITECELL && board[z][y][x] != BLACKCELL) {
        printf("invalid cell\n");
        goto move;
    }
    board[z][y][x] = t;
}

void makerandommove(const char t, const unsigned char s) {
    char z, y, x;
    
    move:
    z = rand() % s;
    y = rand() % s;
    x = rand() % s;
    
    if (board[z][y][x] != WHITECELL && board[z][y][x] != BLACKCELL) {
        goto move;
    }
    
    board[z][y][x] = t;
}

char testwin(const char t, const unsigned char s) {
    unsigned char z, y, x;
    char state = 0; /* 0 = ongoing 1 = tie X = X O = O */
    
    /* linear */
    /* test x */
    for (z = 0; z < s; z++) {
        for (y = 0; y < s; y++) {
            for (x = 0; x < s; x++) {
                if (board[z][y][x] != t) { /* test if all in row belong to team and break on first one that doesn't*/
                    break;
                }
                else if (x == (s - 1)) {
                    state = t;
                    goto end; /* skip all other testing code when the first win condition is found */
                }
            }
        }
    }
    
    /* test y */
    for (z = 0; z < s; z++) {
        for (x = 0; x < s; x++) {
            for (y = 0; y < s; y++) {
                if (board[z][y][x] != t) { 
                    break;
                }
                else if (y == (s - 1)) {
                    state = t;
                    goto end;
                }
            }
        }
    }
    
    /* test z */
    for (y = 0; y < s; y++) {
        for (x = 0; x < s; x++) {
            for (z = 0; z < s; z++) {
                if (board[z][y][x] != t) {
                    break;
                }
                else if (z == (s - 1)) {
                    state = t;
                    goto end;
                }
            }
        }
    }
    
    /* diagonal */
    /* 2D diagonals */
    for (z = 0; z < s; z++) { /* go through each layer */
        for (x = 0; x < s; x++) { /* left diagonals */
            y = x;
            if (board[z][y][x] != t) {
                break;
            }
            else if (x == (s - 1)) {
                state = t;
                goto end;
            }
        }
        
        x = (s - 1);
        for (y = 0; y < s; y++) { /* right diagonals */
            if (board[z][y][x] != t) {
                break;
            }
            else if (y == (s - 1)) {
                state = t;
                goto end;
            }
            x--;
        }
    }
    
    /* 3D diagonals */
    /* z by x */
    for (y = 0; y < s; y++) { 
        for (z = 0; z < s; z++) { /* z++ x++ */
            x = z;
            if (board[z][y][x] != t) {
                break;
            }
            else if (z == (s - 1)) {
                state = t;
                goto end;
            }
        }
        
        z = (s - 1); 
        for (x = 0; x < s; x++) { /* z-- x++ */
            if (board[z][y][x] != t) {
                break;
            }
            else if (x == (s - 1)) {
                state = t;
                goto end;
            }
            z--;
        }
    }
    
    /* z by y */
    for(x = 0; x < s; x++) {
        for (z = 0; z < s; z++) { /* z++ y++ */
            y = z;
            if (board[z][y][x] != t) {
                break;
            }
            else if (z == (s - 1)) {
                state = t;
                goto end;
            } 
        }    
        
        y = (s - 1);
        for (z = 0; z < s; z++) { /* z++ y-- */
            if (board[z][y][x] != t) {
                break;
            }
            else if (z == (s - 1)) {
                state = t;
                goto end;
            } 
            y--;
        }
    }
    
    /* 3D 2-way diagonals */
    /* y++ x++ */
    y = 0;
    x = 0;
    for (z = 0; z < s; z++) {
        if (board[z][y][x] != t) {
            break;
        }
        else if (z == (s - 1)) {
            state = t;
            goto end;
        }
        y++;
        x++;
    }
    
    /* y-- x++ */
    y = (s - 1);
    x = 0;
    for (z = 0; z < s; z++) {
        if (board[z][y][x] != t) {
            break;
        }
        else if (z == (s - 1)) {
            state = t;
            goto end;
        }
        y--;
        x++;
    }
    
    /* y++ x-- */
    y = 0;
    x = (s - 1);
    for (z = 0; z < s; z++) {
        if (board[z][y][x] != t) {
            break;
        }
        else if (z == (s - 1)) {
            state = t;
            goto end;
        }
        y++;
        x--;
    }
    
    /* y-- x-- */
    y = (s - 1);
    x = (s - 1);
    for (z = 0; z < s; z++) {
        if (board[z][y][x] != t) {
            break;
        }
        else if (z == (s - 1)) {
            state = t;
            goto end;
        }
        y--;
        x--;
    }
    
    /* tie */
    for (z = 0; z < s; z++) {
        for (y = 0; y < s; y++) {
            for (x = 0; x < s; x++) {
                if (board[z][y][x] > 88) { /* if unoccupied */
                    goto end;
                }
                else if (z == (s - 1) && y == (s - 1) && x == (s - 1)) { /* if it hasn't broken yet and is at the last cell */
                    state = 1; /* tie game */
                }
            }
        }
    }
    
    end:
    return state;
}

int run(const unsigned char s, const char mode) { /* modes: 0: pvp, 1: pvr, 2: rvr, 3: p v minmax ai, 69: debug*/
    char t = X;
    char outcome = 0; /* win state */
    initializeboard(s);
    drawboard(s);
    
    if (mode == 0) {
        while (outcome == 0) {
            makemove(t, s);
            outcome = testwin(t, s);
            drawboard(s);
        
            if (outcome == 1) {
                printf("tie game");
            }
            else if (outcome > 1) {
                printf("%c wins!", outcome);
            }
        
            if (t == X) {
                t = O;
            }
            else {
                t = X;
            }
        }
    }
    
    else if (mode == 1) {
        while (outcome == 0) {
            if (t == X) {
                makemove(t, s);
            }
            else {
                makerandommove(t, s);
            }
            outcome = testwin(t, s);
            drawboard(s);
        
            if (outcome == 1) {
                printf("tie game");
            }
            else if (outcome > 1) {
                printf("%c wins!", outcome);
            }
        
            if (t == X) {
                t = O;
            }
            else {
                t = X;
            }
        }  
    }
    
    else if (mode == 2) {
        while (outcome == 0) {
            makerandommove(t, s);
            outcome = testwin(t, s);
            drawboard(s);
        
            if (outcome == 1) {
                printf("tie game");
            }
            else if (outcome > 1) {
                printf("%c wins!", outcome);
            }   
        
            if (t == X) {
                t = O;
            }
            else {
                t = X;
            }
        }
    }
    
    else if (mode == 69) {
        while (outcome == 0) {
            makemove(t, s);
            outcome = testwin(t, s);
            drawboard(s);
        
            if (outcome == 1) {
                printf("tie game");
            }
            else if (outcome > 1) {
                printf("%c wins!", outcome);
            }
        }
    }
        
    return outcome;
}

int main(int argc, char *argv[]) {
    srand(time(0));
    run(atoi(argv[1]), atoi(argv[2]));
    return 0;
}