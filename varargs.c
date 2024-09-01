#define _USE_MATH_DEFINES
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// FUCK GCC THOUGH :)

// functions taking variadic number of arguments (optional)
// these arguments can be of the same type or of different types
// these functions must at least have one mandatory argument

static_assert(sizeof(long double) == 8); // sizeof(long double) is 16 with /Qlong-double in icx
static_assert(sizeof(double) == 8);
static_assert(sizeof(float) == 4);

static inline long double __stdcall sum(_In_ const unsigned long long argc, _In_opt_count_(argc)...) {
    assert(argc);
    // the number of optional arguments are determined by the values of the mandatory argument
    // or a special value that terminates the optional list of arguments
    // e.g printf_s() has the format string as the mandatory argument and the number of format specifiers determine the number of optional
    // arguments for consideration

    va_list argptr = NULL;     // the argument pointer (just a typedef for char*)
    __va_start(&argptr, argc); // captures the stack address of the first optional argument
    long double sum = 0.000;
    for (unsigned long long i = 0; i < argc; ++i) sum += va_arg(argptr, int);
    argptr = NULL; // va_end(argptr) just makes the va_list NULL
    return sum;
}

// expects the last optional argument to be 0
static inline long double __stdcall fprod(_In_opt_...) {
    float*      argptr = NULL;
    long double result = 1.0000L;
    __va_start(&argptr, 1000);
    while (*argptr) result *= *argptr++;
    argptr = NULL;
    return result;
}
int wmain(void) {
    // see what happens if we call printf_s with a format string containing three format specifiers and four optional arguments??
    wprintf_s(L"first = %d, second = %.5f, third = %llu\n", 30, (float) M_PI, 45LLU, 2.71828182845905F);
    // the last argument 2.71828182845905F is not considered at all about a missing format specifier
    // clang gave no diagnostics at all with clang .\varargs.c -Wall -Wextra -pedantic -O3 -std=c23
    // unsuprisingly icx behaved just like clang
    // MSVC gave a very detailed diagnostics :)

    const long double total =
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

    const long double tot =
        sum(100,
            731,
            405,
            1621,
            2083,
            1208,
            1430,
            1756,
            806,
            1746,
            1066,
            621,
            698,
            1854,
            963,
            1008,
            2262,
            1508,
            1667,
            173,
            792,
            1674,
            1299,
            857,
            256,
            281,
            365,
            716,
            601,
            1932,
            2230,
            1670,
            104,
            1895,
            1390,
            1263,
            1039,
            2017,
            223,
            1180,
            168,
            1198,
            1987,
            2208,
            277,
            1589,
            856,
            1690,
            685,
            976,
            1667,
            807,
            2284,
            1789,
            739,
            1401,
            980,
            669,
            1793,
            1968,
            1845,
            1464,
            1418,
            101,
            149,
            756,
            2063,
            404,
            1695,
            568,
            1719,
            1444,
            452,
            1734,
            407,
            571,
            921,
            1640,
            2200,
            403,
            1956,
            1871,
            638,
            2125,
            1683,
            391,
            488,
            144,
            2066,
            1713,
            1753,
            1794,
            2274,
            2105,
            1889,
            429,
            992,
            1869,
            308,
            2222,
            1685);

    wprintf_s(L"sum is %Lf\n", total); // 479
    wprintf_s(L"sum is %Lf\n", tot);   // 123470

    const long double product = fprod(
        96.29710262,
        62.86520639,
        4.85969941,
        47.43425343,
        34.0703384,
        46.02342835,
        87.90671681,
        25.73277004,
        91.01933879,
        88.55747657,
        57.69370159,
        88.28465807,
        57.32762841,
        74.83681399,
        77.59659253,
        12.98190632,
        42.71993123,
        24.77670145,
        47.73913958,
        26.17724301,
        53.48384681,
        7.39466115,
        95.74970034,
        96.4951719,
        66.10843315,
        44.22458028,
        34.65685383,
        86.05836633,
        19.87775021,
        43.08600641,
        18.90502342,
        7.84332206,
        1.16875239,
        94.66534665,
        89.85209588,
        73.84995157,
        92.09269413,
        67.63234697,
        51.78705837,
        10.61460114,
        59.74443074,
        43.30582605,
        13.13440423,
        48.96167283,
        43.87142989,
        12.92302787,
        83.49579289,
        42.56180882,
        22.47042904,
        58.58919008,
        96.01019433,
        65.92397513,
        84.14212762,
        61.04321239,
        98.52182804,
        2.41296912,
        82.04477359,
        4.5778807,
        99.17495139,
        25.29167199,
        47.70698989,
        96.9266338,
        21.62853596,
        11.00276935,
        12.60751603,
        98.82741801,
        22.41992726,
        83.3306504,
        81.06096219,
        43.77116463,
        57.92308098,
        78.42640883,
        81.82060524,
        44.51231917,
        21.67363826,
        35.16472,
        11.75346414,
        11.88787835,
        66.97228292,
        19.96633652,
        60.91813389,
        48.60003856,
        83.50027907,
        33.2532476,
        82.99670587,
        80.65894839,
        95.33405508,
        83.50298004,
        27.52043641,
        50.11117041,
        46.94548211,
        17.41544161,
        98.30736353,
        48.39029867,
        79.09835395,
        64.77119917,
        50.2983636,
        42.00310597,
        37.94467582,
        47.07536474,
        0 // va_list traversal terminates @ a NULL arg
    );

    wprintf_s(L"sum is %E\n", product); // 3.6935101603501606E+161

    return EXIT_SUCCESS;
}
