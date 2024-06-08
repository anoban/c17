#include <stdio.h>
#include <stdlib.h>

struct { // anonymous struct
        const wchar_t* const _str;
        unsigned             _len;
} name = { ._str = L"Anoban", ._len = 7 };


int wmain(_In_ int kwargcount, _In_count_(argc) wchar_t* kwargs[], _In_ wchar_t* envvars[]) {
    wprintf_s(L"argc = %2d\n", kwargcount);

    // print out all the arguments
    while (*kwargs) _putws(*kwargs++);

    // print all the environment variables
    while (*envvars) _putws(*envvars++);

    return EXIT_SUCCESS;
}
