// for runtime loading

#define _USE_MATH_DEFINES
#include <math.h>

static long double PI = M_PI;

__declspec(dllexport) long double __cdecl CircleArea(_In_ const long double radius) { return PI * radius * radius; }
