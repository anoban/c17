// test the availability of c23 features

// cl .\clatest.c /std:clatest /Wall /O2 /Ot  /MT /GL
// clang .\clatest.c -Wall -Wextra -pedantic -O3 -std=c23 -march=native

#define __STDC_WANT_SECURE_LIB__ 1
#include <limits.h>
#include <sal.h> // to make gcc happy
#define _USE_MATH_DEFINES
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const size_t HUNDRED_THOUSAND_THOUSAND_WITH_DIGIT_SEPARATOS = 100'000'000;

const unsigned short SHRT_MAXX                              = 0b1111'1111'1111'1111;
static_assert(0b1111'1111'1111'1111 == USHRT_MAX);

[[nodiscard]] static inline long __stdcall square(_In_ const register long __val) { return __val * __val; }

[[nodiscard, deprecated("Use powl instead!")]] static inline long double __stdcall cube(_In_ const register long double __val) {
    return __val * __val * __val;
}

[[unsequenced, nodiscard, reproducible]] double areaofsun(void) { return 4.0000 / 3.00000000 * M_PI * powl(696340.0, 3.0000000000); }

int wmain(void) {
    const unsigned short smax                 = SHRT_MAX;

    [[maybe_unused]] const long double pipow2 = powl(M_PI, 2.00);

    switch ((long) fabsl(ceill(pipow2))) {
        case 10 : _putws(L"10!"); break;
        case 9  : _putws(L"9!"); [[fallthrough]];
        default : break;
    }

    [[maybe_unused]] const typeof_unqual(smax) smin     = SHRT_MIN;
    [[maybe_unused]] const typeof_unqual(pipow2) picube = cube(M_PI);

    [[maybe_unused]] typeof(smax) qualified             = 0;

    // [[maybe_unused]] const void* _ptr                   = nullptr; // MSVC still has no support for nullptr!

    [[maybe_unused]] const volatile bool wrong          = false;
    [[maybe_unused]] typeof(wrong)       right          = true;
    [[maybe_unused]] typeof_unqual(wrong) wrong_too     = false;

    return EXIT_SUCCESS;
}
