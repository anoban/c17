#include <stdint.h>
#include <stdio.h>

size_t wstrlen(_In_ const wchar_t* const restrict string) {
    if (!string) return 0;
    size_t         length = 0;
    const wchar_t* ptr    = string;
    while (*ptr++) length++;
    return length;
}

// this variant cannot accept NULL pointers, [static 1] implies one valid non-NULL pointer is required..
size_t wstrlenstat(_In_ const wchar_t string[static 1] /* this style isn't accepted by MSVC */) {
    if (!string) return 0;
    size_t         length = 0;
    const wchar_t* ptr    = string;
    while (*ptr++) length++;
    return length;
}

int main(void) {
    const wchar_t name[] = L"Anoban";
    wprintf_s(L"wstrlen(name) = %zu\n", wstrlen(name));
    wprintf_s(L"wstrlen(NULL) = %zu\n", wstrlen(NULL));
    wprintf_s(L"wstrlenstat(name) = %zu\n", wstrlenstat(name));
    wprintf_s(L"wstrlenstat(NULL) = %zu\n", wstrlenstat(NULL));
    return 0;
}
