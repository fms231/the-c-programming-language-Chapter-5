#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define NUMBER '0'

int getline(char* s, int lim) {
    int c;
    char* s_start = s;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n') {
        *s++ = c;
    }
    if (c == '\n') {
        *s++ = c;
    }
    *s = '\0';
    return s - s_start;
}

int atoi(char* s) {
    int n, sign;

    for (; isspace(*s); s++) {
        ;
    }
    sign = (*s == '-') ? -1 : 1;
    if (*s == '+' || *s == '-') {
        s++;
    }
    for (n = 0; isdigit(*s); s++) {
        n = 10 * n + (*s - '0');
    }
    return sign * n;
}

void reverse(char* s) {
    int c;
    char *t;

    for (t = s + (strlen(s) - 1); s < t; s++, t--) {
        c = *s;
        *s = *t;
        *t = c;
    }
}

void itoa(int n, char* s) {
    int sign;
    char* s_start = s;

    if ((sign = n) < 0) {
        n = -n;
    }
    do {
        *s++ = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0) {
        *s++ = '-';
    }
    *s = '\0';
    reverse(s_start);
}

int strindex(char* s, char* t) {
    char* s_start = s;
    char* t_start = t;
    char* u;

    for (; *s; s++) {
        for (u = s, t = t_start; *t && *u == *t; u++, t++) {
            ;
        }
        if (t > t_start && !*t) {
            return s - s_start;
        }
    }
    return -1;
}

double atof(char* s) {
    double val, power;
    int sign;

    for (; isspace(*s); s++) {
        ;
    }
    sign = (*s == '-') ? -1 : 1;
    if (*s == '+' || *s == '-') {
        s++;
    }
    for (val = 0.0; isdigit(*s); s++) {
        val = 10.0 * val + (*s - '0');
    }
    if (*s == '.') {
        s++;
    }
    for (power = 1.0; isdigit(*s); s++) {
        val = 10.0 * val + (*s - '0');
        power *= 10.0;
    }
    return sign * val / power;
}

#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;

int getch(void);
void ungetch(int);

int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp >= BUFSIZE) {
        printf("ungetch: too many characters\n");
    } else {
        buf[bufp++] = c;
    }
}

int getop(char* s) {
    int c;

    while ((*s = c = getch()) == ' ' || c == '\t') {
        ;
    }
    *(s + 1) = '\0';
    if (!isdigit(c) && c != '.') {
        return c;
    }
    if (isdigit(c)) {
        while (isdigit(*++s = c = getch())) {
            ;
        }
    }
    if (c == '.') {
        while (isdigit(*++s = c = getch())) {
            ;
        }
    }
    *s = '\0';
    if (c != EOF) {
        ungetch(c);
    }
    return NUMBER;
}