#define _USE_MATH_DEFINES 1
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static __declspec(noinline) void __cdecl square(_Inout_ double* const val) { *val *= *val; }

int wmain(void) {
    // array of arrays cannot be declared with double pointers!
    const char* names[] = { "James", "Janet", "Julian" };
    *names              = "james";

    // names[1][0] = 'j';
    char* const jobs[]  = { "Teacher", "Mason" };

    // *jobs               = "teacher";
    jobs[1][0]          = 'm';

    const double pie    = M_PI;
    square(&pie); // that's lame yuck!

    wprintf_s(L"%lf\n", pie);

    return EXIT_SUCCESS;
}
