// cl .\stringliterals.c /Wall /O2 /Ot /std:clatest /wd4710

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double value = 0.000;

static inline double __cdecl square(typeof(double) arg) { return arg * arg; } // typeof really is decltype huh
static inline double __cdecl cube(typeof(value) arg) { return arg * arg * arg; }

int main(void) {
    char* const    string                 = "STRING LITERAL";
    typeof(string) hello                  = "Hello there!How are ya doin?"; // typeof is poor man's decltype()
    const typeof_unqual(string) samsmith  = "For you............ I had to risk it aaaaaaaaaaall!";

    value                                *= puts(string);
    value                                *= puts(hello);
    value                                *= puts(samsmith);
    value                                *= putchar("\nhalleloojah!"[0]);
    value                                *= printf_s("Square of pi is %F\n", square(M_PI));
    value                                *= printf_s("Cube of pi is %F\n", cube(M_PI));

    return EXIT_SUCCESS;
}
