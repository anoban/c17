#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// i keep forgetting this!!! damn
long double isum(const unsigned _argcount, /* variadic arguments */...) {
    long double total   = 0.0000;
    va_list     _argptr = NULL; // a char* to capture the ADDRESS of the first variadic argument
    va_start(_argptr, 0);       // capture the adddress of the first variadic argument on the stack

    unsigned offset = 0;
    while (offset++ < _argcount) total += va_arg(_argptr, int);
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

    switch (_argtype) {
        case UCHAR : [[fallthrough]];
        case CHAR :
            {
                const char* _ptr = NULL;
                __va_start(&_ptr, 0);
                for (unsigned offset = 0; offset < _argc; ++offset) total += *_ptr++;
                break;
            }
        case USHORT : [[fallthrough]];
        case SHORT :
            {
                const short* _ptr = NULL;
                __va_start(&_ptr, 0);
                for (unsigned offset = 0; offset < _argc; ++offset) total += *_ptr++;
                break;
            }
    }

    return total;
}

int wmain() {
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

    return EXIT_SUCCESS;
}
