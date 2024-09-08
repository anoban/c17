// there are some Microsoft specific idiosyncracies regarding complex support
// MSVC doesn't support STDC complex types.
// it uses structs to emulate complex types.

#define _USE_MATH_DEFINES
#include <complex.h>
#include <math.h>
#include <stdio.h>
#ifdef __STDC_NO_COMPLEX__
    #error "WE NEED COMPLEX TYPES"
#endif

// MS complex.h offers three variants of complex structs
// _Fcomplex with floats for real and imaginary parts
// _Dcomplex with doubles for real and imaginary parts
// _Lcomplex with long doubles for real and imaginary parts
// the _Val[2] member holds the real and imaginary values of the MSVC's _?complex types.

// give a complex type where, the real part has the square root of the argument and the imaginary part holds copy of the argument
static inline _Lcomplex sqrtreal(long double value) {
    return (value < 0.0L) ?
               (_Lcomplex) {
                   ._Val = { 0.0L, sqrtl(-value) }
    } :
               (_Lcomplex) { ._Val = { 0.0L, sqrtl(value) } };
}

int main(void) {
    const _Lcomplex result = sqrtreal(36.735L);
    wprintf_s(L"real: %Lf, imaginary: %Lf\n", result._Val[0], result._Val[1]);
    return 0;
}
