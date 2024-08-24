#include <stdio.h>

extern unsigned expo;

// non static function
extern __declspec(noinline) long double __cdecl power(const double register base, const unsigned register exponent) {
    _putws(__FILEW__ " extern __declspec(noinline) long double __cdecl power(const double register base, const unsigned register exponent)"
    );
    long double result = base;
    for (unsigned i = 0; i < exponent; ++i) result *= base;
    return result;
}

extern __declspec(noinline) void __cdecl call(void) {
    expo = (unsigned) power(expo, 3); // will call the power() function defined in this TU
    wprintf_s(L"expo is now %u\n", expo);
}
