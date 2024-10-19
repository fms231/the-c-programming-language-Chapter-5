#include <stdio.h>

void strncpy(char* s, char* t, int n) {
    while (*t && n-- > 0){
        *s++ = *t++;
    }
    while (n-- > 0) {
        *s++ = '\0';
    }
}

void strncat(char* s, char* t, int n) {
    while (*s) {
        s++;
    }
    while (n-- && (*s++ = *t++)) {
        ;
    }
}

int strncmp(char* s, char* t, int n) {
    while (n-- && *s && *t) {
        if (*s != *t) {
            return *s - *t;
        }
        s++;
        t++;
    }
    return 0;
}

int main() {
    char s[100] = "Hello, ";
    char t[] = "World!";
    strncpy(s, t, 3);
    printf("%s\n", s);
    strncat(s, t, 3);
    printf("%s\n", s);
    printf("%d\n", strncmp(s, t, 3));
    return 0;
}