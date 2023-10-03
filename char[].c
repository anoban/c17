#include <stdio.h>
#include <string.h>
#define NONE void

int main(NONE) {
    char* names[] = { "Julia", "James", "Janet", "Monelle", "Ruby", "Carolle", "Mitchelle", "Sulivan", "Parker" };
    for (int i = 0; i < 9; ++i) printf_s("(%i) %-20s @{%p} len{%llu}\n", i, *(names + i), (names + i), strlen(*(names + i)));
}