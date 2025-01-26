#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// prototype
static long double __declspec(noinline) __stdcall sum_v2(const type _argtype, const unsigned _argc, ...);

// i keep forgetting this!!! damn
long double isum(const unsigned _argcount, /* variadic arguments */...) {
    long double total   = 0.0000;
    va_list     _argptr = NULL; // a char* to capture the ADDRESS of the first variadic argument
    va_start(_argptr, 0);       // capture the adddress of the first variadic argument on the stack

    unsigned i = 0;
    while (i++ < _argcount) total += va_arg(_argptr, int);
    va_end(_argptr);
    return total;
}

static inline long double dprod(const unsigned _argcount, ...) {
    long double   result  = 1.0000;
    const double* _argptr = NULL;
    __va_start(&_argptr, 0); // using the intrinsic
    unsigned _offset = 0;
    while (_offset++ < _argcount) {
        result *= *((double*) _argptr);
        _argptr++; // the type of _argptr will affect how it is incremented!!!!
    }
    return result;
}

typedef enum type { CHAR, UCHAR, SHORT, USHORT, LONG, ULONG, LONGLONG, ULONGLONG, FLOAT, DOUBLE } type;

static long double __declspec(noinline) __stdcall sum(const type _argtype, const unsigned _argc, ...) {
    long double total = 0.000;
    const char* _ptr  = NULL;
    __va_start(&_ptr, _argc);

    switch (_argtype) {
        case UCHAR :
            for (unsigned i = 0; i < _argc; ++i) total += ((unsigned char*) _ptr)[i];
            break;

        case CHAR :
            for (unsigned i = 0; i < _argc; ++i) total += _ptr[i];
            break;

        case USHORT :
            for (unsigned i = 0; i < _argc; ++i) total += ((unsigned short*) _ptr)[i];
            break;

        case SHORT :
            for (unsigned i = 0; i < _argc; ++i) total += ((short*) _ptr)[i];
            break;

        case ULONG :
            for (unsigned i = 0; i < _argc; ++i) total += ((unsigned long*) _ptr)[i];
            break;

        case LONG :
            for (unsigned i = 0; i < _argc; ++i) total += ((long*) _ptr)[i];
            break;

        case ULONGLONG :
            for (unsigned i = 0; i < _argc; ++i)
                total += ((unsigned long long*) _ptr)[i]; // NOLINT(cppcoreguidelines-narrowing-conversions)
            break;

        case LONGLONG :
            for (unsigned i = 0; i < _argc; ++i) total += ((long long*) _ptr)[i]; // NOLINT(cppcoreguidelines-narrowing-conversions)
            break;

        case FLOAT :
            // {
            //     const float* _ptr = NULL;
            //     __va_start(&_ptr, _argc);
            //     for (unsigned i = 0; i < _argc; ++i) total += _ptr[i];
            //     break;
            // }
            [[fallthrough]]; // WOW!!!
        case DOUBLE :
            {
                const double* _ptr = NULL;
                __va_start(&_ptr, _argc);
                for (unsigned i = 0; i < _argc; ++i) total += _ptr[i];
                break;
            }
    }

    return total;
}

int wmain(void) {
    wprintf_s(
        L"sum of 8,58,31,45,41,48,10,8,30,61,55,41,33,67,23,85,50,16,48,18 is %.4Lf\n",
        isum(20, 8, 58, 31, 45, 41, 48, 10, 8, 30, 61, 55, 41, 33, 67, 23, 85, 50, 16, 48, 18) // 776???
    );

    wprintf_s(
        L"1.22372719, 1.9661265, 1.56248383, 1.27414079, 1.4696337, 1.35327028, 1.83275722, 1.74608856, 1.23482471, 1.16469709, 1.24552609, 1.92986116, 1.31685582, 1.96357836, 1.31069161, 1.81417854, 1.81566778, 2.0486647, 1.11827143, 1.86500132 is %.5Lf\n",
        dprod(
            20,
            1.22372719,
            1.9661265,
            1.56248383,
            1.27414079,
            1.4696337,
            1.35327028,
            1.83275722,
            1.74608856,
            1.23482471,
            1.16469709,
            1.24552609,
            1.92986116,
            1.31685582,
            1.96357836,
            1.31069161,
            1.81417854,
            1.81566778,
            2.0486647,
            1.11827143,
            1.86500132
        )
    ); // 5026.801280741084????

    // 1325!!!
    wprintf(L"sum is %.5Lf\n", sum(UCHAR, 10, 213UI8, 146UI8, 186UI8, 106UI8, 8UI8, 224UI8, 192UI8, 12UI8, 69UI8, 169UI8));
    // 152!!!
    wprintf(L"sum is %.5Lf\n", sum(CHAR, 10, 27I8, 66I8, -31I8, -15I8, 84I8, -100I8, 58I8, -128I8, 78I8, 113I8));
    wprintf(L"sum is %.5Lf\n", sum(USHORT, 10, 213UI16, 146UI16, 186UI16, 106UI16, 8UI16, 224UI16, 192UI16, 12UI16, 69UI16, 169UI16));
    wprintf(L"sum is %.5Lf\n", sum(SHORT, 10, 27I16, 66I16, -31I16, -15I16, 84I16, -100I16, 58I16, -128I16, 78I16, 113I16));
    wprintf(L"sum is %.5Lf\n", sum(ULONG, 10, 213LU, 146LU, 186LU, 106LU, 8LU, 224LU, 192LU, 12LU, 69LU, 169LU));
    wprintf(L"sum is %.5Lf\n", sum(LONG, 10, 27, 66, -31, -15, 84, -100, 58, -128, 78, 113));
    wprintf(L"sum is %.5Lf\n", sum(ULONGLONG, 10, 213LLU, 146LLU, 186LLU, 106LLU, 8LLU, 224LLU, 192LLU, 12LLU, 69LLU, 169LLU));
    wprintf(L"sum is %.5Lf\n", sum(LONGLONG, 10, 27LL, 66LL, -31LL, -15LL, 84LL, -100LL, 58LL, -128LL, 78LL, 113LL));

    _putws(L"\nreal numbers!\n");

    wprintf(L"sum is %.5Lf\n", sum(FLOAT, 10, 27.00F, 66.00F, -31.00F, -15.00F, 84.00F, -100.00F, 58.00F, -128.00F, 78.00F, 113.00F));
    wprintf(L"sum is %.5Lf\n", sum(DOUBLE, 10, 27.00, 66.00, -31.00, -15.00, 84.00, -100.00, 58.00, -128.00, 78.00, 113.00));
    return EXIT_SUCCESS;
}

static long double __declspec(noinline) __stdcall sum_v2(const type _argtype, const unsigned _argc, ...) {
    long double total = 0.000;

    switch (_argtype) {
        // unsigned type group subject to type promotion
        case UCHAR  : [[fallthrough]];
        case USHORT : [[fallthrough]];
        case ULONG :
            {
                const unsigned long* _ptr = NULL;
                __va_start(&_ptr, _argc);
                for (unsigned i = 0; i < _argc; ++i) total += _ptr[i];
                break;
            }
            // signed type group subject to type promotion
        case CHAR  : [[fallthrough]];
        case SHORT : [[fallthrough]];
        case LONG :
            {
                const long* _ptr = NULL;
                __va_start(&_ptr, _argc);
                for (unsigned i = 0; i < _argc; ++i) total += _ptr[i];
                break;
            }

        case ULONGLONG :
            for (unsigned i = 0; i < _argc; ++i)
                total += ((unsigned long long*) _ptr)[i]; // NOLINT(cppcoreguidelines-narrowing-conversions)
            break;

        case LONGLONG :
            for (unsigned i = 0; i < _argc; ++i) total += ((long long*) _ptr)[i]; // NOLINT(cppcoreguidelines-narrowing-conversions)
            break;

        case FLOAT : [[fallthrough]];
        case DOUBLE :
            {
                const double* _ptr = NULL;
                __va_start(&_ptr, _argc);
                for (unsigned i = 0; i < _argc; ++i) total += _ptr[i];
                break;
            }
    }

    return total;
}
