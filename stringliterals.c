// cl .\stringliterals.c /Wall /O2 /Ot /std:clatest /wd4710

#define _USE_MATH_DEFINES // M_PI
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <uchar.h>
#include <wchar.h>

#ifndef __STDC_UTF_16__
    #error "UTF-16 strings are not supported!"
#endif

#ifndef __STDC_UTF_32__
    #error "UTF-32 strings are not supported!"
#endif

double value = 0.000;

static inline double __cdecl square(typeof(double) arg) { return arg * arg; }
static inline double __cdecl cube(typeof(value) arg) { return arg * arg * arg; }

static const typeof(M_PI) PI = M_PI;

int main(void) {
    char* const string                    = "STRING LITERAL";
    string[0]                             = 's';

    typeof(string) hello                  = "Hello there!How are ya doin?"; // typeof is poor man's decltype()
    const typeof_unqual(string) samsmith  = "For you............ I had to risk it aaaaaaaaaaall!";

    value                                *= puts(string);
    value                                *= puts(hello);
    value                                *= puts(samsmith);
    value                                *= putchar("\nThe writing's on the waaaallllllllllllll!"[0]);
    value                                *= printf_s("Square of pi is %F\n", square(M_PI));
    value                                *= printf_s("Cube of pi is %F\n", cube(M_PI));

    puts(&"Hi there!");
    puts("Hi there!");

    static const wchar_t* const restrict NAME   = L"Anoban";
    static const char16_t* const restrict uNAME = u"Anoban";
    static const char32_t* const restrict UNAME = U"Anoban";

    return EXIT_SUCCESS;
}
