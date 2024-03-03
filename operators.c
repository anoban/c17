#include <limits.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    size_t max = SIZE_MAX;
    wprintf_s(L"SIZE_MAX is %zu and SIZE_MAX++ is %zu\n", max, max + 1); // wrap aorund addition

    int number = SHRT_MAX;
    wprintf_s(L"SHRT_MAX is %hd\n", SHRT_MAX);
    wprintf_s(L"SHRT_MAX++ is %d\n", number++);
    --number;
    wprintf_s(L"++SHRT_MAX is %d\n", ++number);                          // %hd will show the result as -SHRT_MAX, wrap around addition :)

    int a, b = 6748, c, d;

    a = b * powl(M_PI, 26.7532);
    c = d = powl(a, M_PI) + abs(M_PI);
    return 0;
}
