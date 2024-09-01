#define _USE_MATH_DEFINES
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// functions taking variadic number of arguments (optional)
// these arguments can be of the same type or of different types
// these functions must at least have one mandatory argument

typedef float       float32;
typedef long double float64;

static_assert(sizeof(long double) == 8); // sizeof(long double) is 16 with /Qlong-double
static_assert(sizeof(double) == 8);
static_assert(sizeof(float) == 4);

static inline float64 __stdcall sum(_In_ const unsigned long long argc, _In_opt_count_(argc)...) {
    assert(argc);
    // the number of optional arguments are determined by the values of the mandatory argument
    // or a special value that terminates the optional list of arguments
    // e.g printf_s() has the format string as the mandatory argument and the number of format specifiers determine the number of optional
    // arguments for consideration

    va_list argptr = NULL;     // the argument pointer (just a typedef for char*)
    __va_start(&argptr, argc); // captures the stack address of the first optional argument
    float64 sum = 0.000;
    for (unsigned long long i = 0; i < argc; ++i) sum += va_arg(argptr, int);
    argptr = NULL; // va_end(argptr) just makes the va_list NULL
    return sum;
}

int wmain(void) {
    // see what happens if we call printf_s with a format string containing three format specifiers and four optional arguments??
    wprintf_s(L"first = %d, second = %.5f, third = %llu\n", 30, (float) M_PI, 45LLU, 2.71828182845905F);
    // the last argument 2.71828182845905F is not considered at all about a missing format specifier
    // clang gave no diagnostics at all with clang .\varargs.c -Wall -Wextra -pedantic -O3 -std=c23
    // unsuprisingly icx behaved just like clang
    // MSVC gave a very detailed diagnostics :)

    const float64 total = // should be 479
        sum(100,
            7,
            6,
            3,
            5,
            3,
            4,
            4,
            4,
            7,
            3,
            5,
            0,
            4,
            6,
            4,
            5,
            1,
            8,
            2,
            8,
            5,
            3,
            6,
            4,
            8,
            7,
            0,
            4,
            7,
            6,
            8,
            6,
            8,
            1,
            2,
            8,
            9,
            1,
            5,
            0,
            8,
            0,
            1,
            7,
            8,
            2,
            8,
            8,
            6,
            1,
            6,
            9,
            8,
            3,
            4,
            1,
            9,
            3,
            9,
            1,
            7,
            1,
            1,
            4,
            8,
            8,
            3,
            6,
            3,
            7,
            9,
            1,
            0,
            1,
            6,
            6,
            9,
            4,
            5,
            2,
            9,
            6,
            6,
            8,
            5,
            9,
            1,
            6,
            4,
            3,
            6,
            9,
            5,
            1,
            5,
            1,
            4,
            1,
            5,
            5);

    wprintf_s(L"sum is %Lf\n", total);

    return EXIT_SUCCESS;
}
