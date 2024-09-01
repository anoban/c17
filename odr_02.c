#include <sal.h>
#include <stdio.h>

extern unsigned expo; // defined in odr_01.c

// non static function
extern __declspec(noinline) long double __cdecl power(_In_ register const double base, _In_ register const unsigned exponent) {
    _putws(__FILEW__ " :: " __FUNCTIONW__);
    long double result = base;
    for (unsigned i = 1; i < exponent; ++i) result *= base;
    return result;
}

static inline void __cdecl say_hi(void) { _putws(L"Hi from " __FILEW__ "::" __FUNCTIONW__); }

// non static function
extern __declspec(noinline) void __cdecl call(void) {
    _putws(L"inside " __FUNCTIONW__);
    expo = (unsigned) power(expo, 3); // will call the power() function defined in this TU
    // because the power() function defined in odr_01.c is a static function
    wprintf_s(L"expo is now 12^3 (%u)\n", expo);
}
