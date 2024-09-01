#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int wmain(void) {
    double           pi          = M_PI;
    long long* const as_integer  = &pi; // not a hard error in clang, MSVC and icx but errs with gcc
    *as_integer                 *= 2.000;

    wprintf_s(L"%lf\n", pi);

    return EXIT_SUCCESS;
}
