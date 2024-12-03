#ifndef __llvm__
    #error NEEDS AN LLVM BASED COMPILER WITH SUPPORT FOR C23
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define __TOSTR(EXPANDED_MACRO)     #EXPANDED_MACRO
#define TO_STRING(UNEXPANDED_MACRO) __TOSTR(UNEXPANDED_MACRO)

static inline double __stdcall sum(_In_opt_...) { // first argument must specify the number of args to sum
    // since variadic arguments do not have a prespecified type, they MUST USE default types when dereferenced inside functions using
    // the argument pointer i.e integer arguments must be dereferenced as int or unsigned int
    // and floating point arguments must be dereferenced as double!!!
    double  sum      = 0.0;
    va_list args_ptr = nullptr;

    // prior to C23, va_start() took a va_list and the expected argument count as arguments
    // since C23, va_start() evaluates only the first argument, i.e va_start() macro can be invoked with a single argument
    __va_start(&args_ptr, 0); // and will map to __va_start(&args_ptr, 0); with /std:c23 flag

    // until we reach 0 (the last argument)
    const unsigned argc = va_arg(args_ptr, unsigned);
    for (unsigned i = 0; i < argc; ++i) sum += va_arg(args_ptr, double);
    args_ptr = NULL; // equivalent to  va_end(args_ptr)
    return sum;
}

static inline long double __stdcall ssum(_In_opt_...) {
    char*       argv   = nullptr;
    long double result = 0.0000;

    wprintf_s(L"%0X\n", argv);
    __va_start(&argv, 0); // capture the first argument pointer
    wprintf_s(L"%0X\n", argv);
    // wprintf_s(L"%Lf\n", *(double*) argv);

    // until the argument is NULL (0), continue to process the arguments and sum them
    // while (*((int*) argv))
    for (unsigned i = 0; i < 6; ++i) { // works :)
        wprintf_s(L"sum is = %Lf\n", result);
        result += *((double*) argv);
        argv   += 8;
    }

    return result;
}

int wmain(void) {
    const double sixty = sum(6, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F); // explicitly passing floats doesn't help with the
                                                                           // va_list dereferencing!
    // when using non-default types, clang says  second argument to 'va_arg' is of promotable type 'float'; this va_arg has undefined behavior
    // because arguments will be promoted to 'double'
    wprintf_s(L"sum is = %lf\n", sixty);

    const long double ssixty = ssum(10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 0); // parse arguments untill NULL logic does no
    wprintf_s(L"sum is = %lf\n", ssixty);

    return EXIT_SUCCESS;
}
