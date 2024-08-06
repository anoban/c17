#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double value = 0.000;

static inline double __cdecl square(typeof(value) arg) { return arg * arg; }

int main(void) {
    const char* const string                    = "STRING LITERAL";
    typeof(string)    hello                     = "Hello there!How are ya doin?"; // typeof is poor man's decltype()
    const typeof_unqual(string) const samsmith  = "For you............ I had to risk it aaaaaaaaaaall!";

    value                                      *= puts(string);
    value                                      *= puts(hello);
    value                                      *= puts(samsmith);
    value                                      *= putchar("\nhalleloojah!"[0]);
    value                                      *= printf_s("Square of pi is %Lf\n", square(M_PI));

    return EXIT_SUCCESS;
}
