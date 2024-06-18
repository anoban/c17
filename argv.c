#include <stdio.h>
#include <stdlib.h>

int wmain(_In_opt_ const int argc, _In_opt_count_(argc) const wchar_t* const argv[]) {
    wprintf_s(L"argc = %d\n", argc);
    for (int i = 0; i < argc; ++i) wprintf_s(L"Argument no %d: %s\n", i, argv[i]);
    return EXIT_SUCCESS;
}
