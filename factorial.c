#include <stdint.h>
#include <stdio.h>

// recursive solution
long double factorial(_In_ const int64_t number) {
    if (number < 0) return 0.000L;
    if (!number) return 1.000L;
    return number * factorial(number - 1);
}

int main(void) {
    const long double result = factorial(100);
    wprintf_s(L"factorial of %lld is %10.5Lf\n", 100, result);
    return 0;
}
