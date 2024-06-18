#include <stdio.h>

int wmain(void) {
    unsigned i = 11, j = 1;
    for (; i < 10000; i += 100) j *= i;
    wprintf_s(L"i = %u\n", i);
    return 0;
}
