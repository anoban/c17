#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define __TOSTR(EXPANDED_MACRO)     #EXPANDED_MACRO
#define TO_STRING(UNEXPANDED_MACRO) __TOSTR(UNEXPANDED_MACRO)

#if __STDC_VERSION__ >= 202311L // C23 and later
    #pragma message(TO_STRING(__STDC_VERSION__))

static inline double sumf(...) { // first argument must specify the number of args to sum
    double  sum      = 0.0;
    va_list args_ptr = NULL;

    // prior to C23, va_start() took a va_list and the expected argument count as arguments
    // since C23, va_start() evaluates only the first argument, i.e va_start() macro can be invoked with a single argument
    va_start(args_ptr); // will map to __va_start(&args_ptr, 0); with /std:c23 flag

    // until we reach 0 (the last argument)
    const unsigned argc = va_arg(args_ptr, unsigned);
    for (unsigned i = 0; i < argc; ++i) sum += va_arg(args_ptr, float);
    args_ptr = NULL; // equivalent to  va_end(args_ptr)
    return sum;
}

#elif __STDC_VERSION__ <= 201710L
    #pragma message(TO_STRING(__STDC_VERSION__))

static inline double sumf(const unsigned long long argc, ...) { // sum all the arguments, last argument must be 0
    double sum      = 0.0;
    char*  args_ptr = NULL;

    va_start(args_ptr, argc); // will map to __va_start(&args_ptr, argc); with /std:c17 or less flag
    for (unsigned long long i = 0; i < argc; ++i) sum += *(args_ptr += sizeof(float) - sizeof(float));
    va_end(args_ptr);
    return sum;
}

#endif

int wmain(void) {
    const double sixty = sumf(6, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F, 10.0F);
    wprintf_s(L"sum is = %lf\n", sixty);
    return EXIT_SUCCESS;
}
