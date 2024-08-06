// clang .\staticv.c -O3 -Wall -Wextra -pedantic -std=c23

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

static inline unsigned __cdecl increment() {
    static long count = 0;
    wprintf_s(L"count = %ld\n", count);
    return ++count;
}

static inline const long double* __cdecl static_const_address() {
    const static long double pie = M_PI; // works ;)
    return &pie;
}

static inline long double* __cdecl static_address() {
    static long double pie = M_PI; // works too ;)
    return &pie;
}

int wmain(void) {
    for (unsigned i = 0; i < 10; ++i) increment();
    wprintf_s(L"Is that pi? %Lf\n", *static_const_address());

    *static_address() *= 2.000;
    wprintf_s(L"Is that 2 x pi? %Lf\n", *static_address()); // gotta love C <3

    return 0;
}
