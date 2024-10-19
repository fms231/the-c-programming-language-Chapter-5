#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES   5000
#define MAXLEN     1000
#define ALLOCSIZE  10000
#define NUMERIC    01
#define REVERSE    02
#define FOLD       04
#define DIRORDER   010
#define FIELD      020

typedef int (*funcP)(void *, void*);

int getLine(char *, int);
int readlines(char *[], int);
void witelines(char *[], int, int);
char *alloc(int);
void qSort(void *[], int, int, funcP);
int strCmp(char *, char *);
int numcmp(char *, char *);
int foldCmp(char *, char *);
int dirCmp(char *, char *);
int expandArg(int, char **);
int fieldCmp(char *, char *);
funcP comparator(int);

/* globals */
static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;
static char option;
static long fieldpos;

int getLine(char *s, int lim)
{
    int c, len;

    len = 0;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n') {
        *s++ = c;
        ++len;
    }
    if (c == '\n') {
        *s++ = c;
        ++len;
    }
    *s = '\0';
    return len;
}

int readlines(char *lineptr[], int maxlines)
{
    int len, nlines;
    char *p, line[MAXLEN];

    nlines = 0;
    while ((len = getLine(line, MAXLEN)) > 0) {
        if (nlines >= maxlines || (p = alloc(len)) == NULL)
            return -1;
        line[len - 1] = '\0';
        strcpy(p, line);
        lineptr[nlines++] = p;
    }
    return nlines;
}

void writelines(char *lineptr[], int nlines, int reverse)
{
    if (reverse)
        while (--nlines > 0)
            printf("%s\n", lineptr[nlines]);
    else
        while (nlines-- > 0)
            printf("%s\n", *lineptr++);
}

char *alloc(int n)
{
    if (allocbuf + ALLOCSIZE - allocp < n)
        return 0;
    allocp += n;
    return allocp - n;
}

void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

void qSort(void *v[], int left, int right, funcP comp)
{
    int i, last;

    void swap(void *v[], int, int);

    if (left >= right)
        return;
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++)
        if ((*comp)(v[i], v[left]) < 0)
            swap(v, ++last, i);
    swap(v, left, last);
    qSort(v, left, last - 1, comp);
    qSort(v, last + 1, right, comp);
}

int strCmp(char *s, char *t)
{
    for ( ; *s == *t; s++, t++)
        if (*s == '\0')
            return 0;
    return *s - *t;
}

int numcmp(char *s1, char *s2)
{
    double v1, v2;

    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2)
        return -1;
    if (v1 > v2)
        return 1;
    return 0;
}

int foldCmp(char *s, char *t)
{
    char str1[MAXLEN], str2[MAXLEN];
    int i;
    funcP fp;

    for (i = 0; *s; ++s)
        str1[i++] = tolower(*s);
    str1[i] = '\0';
    for (i = 0; *t; ++t)
        str2[i++] = tolower(*t);
    str2[i] = '\0';
    fp = comparator(option & DIRORDER);
    return fp(str1, str2);
}

int dirCmp(char *s, char *t)
{
    int i;
    char str1[MAXLEN], str2[MAXLEN];

    for (i = 0; *s != '\0'; ++s)
        if (isalnum(*s) || isblank(*s))
            str1[i++] = *s;
    str1[i] = '\0';
    for (i = 0; *t != '\0'; ++t)
        if (isalnum(*t) || isblank(*t))
            str2[i++] = *t;
    str2[i] = '\0';
    return strCmp(str1, str2);
}

int fieldCmp(char *s, char *t)
{
    int i;
    funcP fp;

    for (i = 1; i < fieldpos && *s; ++s, ++i) {
        while (!isblank(*s) && *s)
            ++s;
        while (isblank(*s))
            ++s;
        --s;
    }
    for (i = 1; i < fieldpos && *t; ++t, ++i) {
        while (*t && !isblank(*t) && *t)
            ++t;
        while (isblank(*t))
            ++t;
        --t;
    }
    fp = comparator(option ^ FIELD);
    return fp(s, t);
}

funcP comparator(int option)
{
    return (funcP) (option & FIELD ? fieldCmp : option & NUMERIC? numcmp :
                                                option & FOLD ? foldCmp : option & DIRORDER ? dirCmp : strCmp);
}

int expandArg(int count, char **list)
{
    char *prog, *endptr;
    int c, parseK;

    prog = *list; /* program name */
    option = parseK = 0;
    endptr = NULL;

    while (--count > 0 && **++list == '-') {
        while ((c = *++*list) && !parseK) {
            switch (c) {
                case ('n'):
                    option |= NUMERIC;
                    break;
                case ('r'):
                    option |= REVERSE;
                    break;
                case ('f'):
                    option |= FOLD;
                    break;
                case ('d'):
                    option |= DIRORDER;
                    break;
                case ('k'):
                    option |= FIELD;
                    parseK = 1;
                    break;
                default:
                    printf("%s: invalid option -- '%c'\n", prog, c);
                    count = -1;
                    break;
            }
        }
        if (parseK) {
            if (isdigit(c))
                fieldpos = strtol(*list, &endptr, 0);
            else if (c == '\0' && count > 1) {
                fieldpos = strtol(*++list, &endptr, 0);
                --count;
            } else
                count = -1;
            parseK = 0;
        }
    }
    if (option & NUMERIC && option & DIRORDER) {
        printf("%s: options '-dn' are incompatible\n", prog);
        count = -1;
    }
    if (option & FIELD && (!fieldpos || *endptr)) {
        printf("%s: option requires an argument -- 'k'\n", prog);
        count = -1;
    }
    if (count != 0) {
        printf("Usage: %s [-nr] [-fdr] [-k field]\n", prog);
        return -1;
    }
    return 0;
}

// 命令行参数新增 -k 表示按字段进行排序
// 例如 -k 3 表示按第三个字段进行排序
int main(int argc, char *argv[])
{
    char *lineptr[MAXLINES];
    int nlines;

    if (expandArg(argc, argv) < 0)
        return 1;
    if ((nlines = readlines(lineptr, MAXLINES)) < 0) {
        printf("input too big to sort\n");
        return 1;
    }
    qSort((void**) lineptr, 0, nlines - 1, comparator(option));
    writelines(lineptr, nlines, option & REVERSE);
    return 0;
}