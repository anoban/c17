#include <stdio.h>

int main(void) {
    for (size_t start = 80, stop = 100; stop > start; start++) wprintf_s(L"%zu\n", start);
    _putws(L"");
    _putws(L"");
    for (size_t start = 80, stop = 100; stop > start; start++, stop--) wprintf_s(L"%zu\n", start);
    return 0;
}
