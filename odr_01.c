#define _USE_MATH_DEFINES
#include <math.h>
#include <sal.h>
#include <stdio.h>
#include <stdlib.h>

extern __declspec(noinline) void __cdecl call(void); // definition in odr_02.c

// static function
static __declspec(noinline) double power(_In_ register const double base, _In_ register const unsigned exponent) {
    _putws(__FILEW__ " :: " __FUNCTIONW__);
    double result = base;
    for (unsigned i = 1; i < exponent; ++i) result *= base;
    return result;
}

// implicitly extern
inline void __cdecl say_hi(void) { _putws(L"Hi from " __FILEW__ "::" __FUNCTIONW__); }

unsigned expo = 12; // globally accessible

int main(void) {
    const long double res = power(M_PI, expo); // will call the power() function defined in this TU even though
    // the non-static power() function defined in odr_02.c will be visible during the link time
    printf_s("%.6lf raised to the power %u is %.6Lf\n", M_PI, expo, res);
    call();
    return EXIT_SUCCESS;
}
