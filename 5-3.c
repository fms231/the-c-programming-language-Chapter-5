#include <stdio.h>

void stract(char* s, char* t) {
    while (*s) {
        s++;
    }
    while (*s++ = *t++) {
        ;
    }
}

int main() {
    char s[100] = "hello, ";
    char t[100] = "world!";
    stract(s, t);
    printf("%s\n", s);
    return 0;
}