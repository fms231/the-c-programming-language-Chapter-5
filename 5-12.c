#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 100
#define TABINC 4
#define YES 1
#define NO 0

void esettab(int argc, char* argv[], char* tab);
void entab(char* tab);
int tabpos(int pos, char* tab);

// 命令行参数为 -m +n 例如 -4 +4
int main(int argc, char* argv[]) {
    char tab[MAXLINE + 1];

    esettab(argc, argv, tab);
    entab(tab);
    return 0;
}

int tabpos(int pos, char* tab){
    if (pos > MAXLINE){
        return YES;
    } else {
        return tab[pos];
    }
}

void entab(char *tab){
    int c, pos;
    int nb = 0;
    int nt = 0;

    for (pos = 1; (c = getchar()) != EOF; pos++){
        if (c == ' '){
            if (tabpos(pos, tab) == NO){
                ++nb;
            } else {
                nb = 0;
                ++nt;
            }
        } else {
            for (; nt > 0; nt--){
                putchar('\t');
            }
            if (c == '\t'){
                nb = 0;
            } else {
                for (; nb > 0; nb--){
                    putchar(' ');
                }
            }
            putchar(c);
            if (c == '\n'){
                pos = 0;
            } else if (c == '\t'){
                while (tabpos(pos, tab) != YES){
                    ++pos;
                }
            }
        }
    }
}

void esettab(int argc, char* argv[], char* tab) {
    int i, pos, inc;

    if (argc <= 1) {
        for (i = 1; i <= MAXLINE; i++) {
            if (i % TABINC == 0) {
                tab[i] = YES;
            } else {
                tab[i] = NO;
            }
        }
    } else if (argc == 3 && *argv[1] == '-' && *argv[2] == '+') {
        pos = atoi(&(*++argv)[1]);
        inc = atoi(&(*++argv)[1]);
        for (i = 1; i <= MAXLINE; i++) {
            if (i != pos) {
                tab[i] = NO;
            } else {
                tab[i] = YES;
                pos += inc;
            }
        }
    } else {
        for (i = 1; i <= MAXLINE; i++) {
            tab[i] = NO;
        }
        while (--argc > 0) {
            pos = atoi(*++argv);
            if (pos > 0 && pos <= MAXLINE) {
                tab[pos] = YES;
            }
        }
    }
}