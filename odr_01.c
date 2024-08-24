#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

extern __declspec(noinline) void __cdecl call(void); // definition in odr_02.c

// static function
static __declspec(noinline) double power(const double register base, const unsigned register exponent) {
    _putws(L"static __declspec(noinline) double power(const double register base, const unsigned register exponent)");
    double result = base;
    for (unsigned i = 0; i < exponent; ++i) result *= base;
    return result;
}

unsigned expo = 12; // globally accessible

int wmain(void) {
    const long double res = power(M_PI, expo);
    wprintf_s(L"%.6Lf raised to the power %u is %.6Lf\n", M_PI, expo, res);
    call();
    return EXIT_SUCCESS;
}
