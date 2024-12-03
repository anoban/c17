#include <stdio.h>

typedef struct {
        char* names[4];
} faves_t;

int main(void) {
    faves_t Languages = {
        .names = { "Python", "C", "Fortran", "x86-64 asm" }
    };
    faves_t Tech = {
        .names = { "Win32", "MSVC", "oneAPI", "CUDA" }
    };

    for (int i = 0; i < 4; ++i) printf_s("%s, %s\n", Languages.names[i], Tech.names[i]);
    return 0;
}
