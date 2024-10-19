#include <string.h>
#include <stdio.h>

#define MAXLEN 1000
#define MAXSTOP 5000

int getline(char* s, int lim) {
    int c, i;
    i = 0;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n') {
        s[i++] = c;
    }
    if (c == '\n') {
        s[i++] = c;
    }
    s[i] = '\0';
    return i;
}

int readlines(char* lineptr[], char* linestor, int maxlines) {
    int len, nlines;
    char line[MAXLEN];
    char* p = linestor;
    char* linestop = linestor + MAXSTOP;

    nlines = 0;
    while ((len = getline(line, MAXLEN)) > 0) {
        if (nlines >= maxlines || p + len > linestop) {
            return -1;
        } else {
            line[len - 1] = '\0';
            strcpy(p, line);
            lineptr[nlines++] = p;
            p += len;
        }
    }
    return nlines;
}