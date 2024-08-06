#include <stdio.h>

static inline unsigned __cdecl addone() {
    static long count = 0;
    wprintf_s(L"count = %ld\n", count);

    count++;
    return count;
}

int wmain(void) {
    for (unsigned i = 0; i < 10; ++i) addone();
    return 0;
}
