#include <stdio.h>

int strend(char* s, char* t) {
    char* s_start = s;
    char* t_start = t;
    while (*s) {
        s++;
    }
    while (*t) {
        t++;
    }
    while (s >= s_start && t >= t_start) {
        if (*s != *t) {
            return 0;
        }
        s--;
        t--;
    }
    if (t < t_start) {
        return 1;
    }
    return 0;
}

int main() {
    char s[] = "Hello, World!";
    char t[] = "World!";
    printf("%d\n", strend(s, t));
    return 0;
}