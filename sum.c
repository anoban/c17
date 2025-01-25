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
        case UCHAR :
            {
                const unsigned char* _ptr = NULL;
                __va_start(&_ptr, _argc);
                for (unsigned offset = 0; offset < _argc; ++offset) total += *_ptr++;
                break;
            }
        case CHAR :
            {
                const char* _ptr = NULL;
                __va_start(&_ptr, _argc);
                for (unsigned offset = 0; offset < _argc; ++offset) total += *_ptr++;
                break;
            }
        case USHORT :
            {
                const unsigned short* _ptr = NULL;
                __va_start(&_ptr, _argc);
                for (unsigned offset = 0; offset < _argc; ++offset) total += *_ptr++;
                break;
            }
        case SHORT :
            {
                const short* _ptr = NULL;
                __va_start(&_ptr, _argc);
                for (unsigned offset = 0; offset < _argc; ++offset) total += *_ptr++;
                break;
            }
        case ULONG :
            {
                const unsigned long* _ptr = NULL;
                __va_start(&_ptr, _argc);
                for (unsigned offset = 0; offset < _argc; ++offset) total += *_ptr++;
                break;
            }
        case LONG :
            {
                const long* _ptr = NULL;
                __va_start(&_ptr, _argc);
                for (unsigned offset = 0; offset < _argc; ++offset) total += *_ptr++;
                break;
            }
        case ULONGLONG :
            {
                const unsigned long long* _ptr = NULL;
                __va_start(&_ptr, _argc);
                for (unsigned offset = 0; offset < _argc; ++offset) total += *_ptr++;
                break;
            }
        case LONGLONG :
            {
                const long long* _ptr = NULL;
                __va_start(&_ptr, _argc);
                for (unsigned offset = 0; offset < _argc; ++offset) total += *_ptr++;
                break;
            }
        case FLOAT :
            {
                const float* _ptr = NULL;
                __va_start(&_ptr, _argc);
                for (unsigned offset = 0; offset < _argc; ++offset) total += *_ptr++;
                break;
            }
        case DOUBLE :
            {
                const double* _ptr = NULL;
                __va_start(&_ptr, _argc);
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

    // 1325!!!
    wprintf(L"sum is %.5Lf\n", sum(UCHAR, 10, 213UI8, 146UI8, 186UI8, 106UI8, 8UI8, 224UI8, 192UI8, 12UI8, 69UI8, 169UI8));
    // 152!!!
    wprintf(L"sum is %.5Lf\n", sum(CHAR, 10, 27I8, 66I8, -31I8, -15I8, 84I8, -100I8, 58I8, -128I8, 78I8, 113I8));
    wprintf(L"sum is %.5Lf\n", sum(USHORT, 10, 213UI16, 146UI16, 186UI16, 106UI16, 8UI16, 224UI16, 192UI16, 12UI16, 69UI16, 169UI16));
    wprintf(L"sum is %.5Lf\n", sum(SHORT, 10, 27I16, 66I16, -31I16, -15I16, 84I16, -100I16, 58I16, -128I16, 78I16, 113I16));

    return EXIT_SUCCESS;
}
