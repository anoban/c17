#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

static inline noreturn void __stdcall woohoo(void) { _putws(L"Woohooo!"); }

int main(void) {
    _putws(L"Inside main");
    atexit(woohoo); // atexit registration
    return EXIT_SUCCESS;
}
