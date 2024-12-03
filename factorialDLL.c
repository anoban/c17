#include <assert.h>

unsigned long long __declspec(dllexport) __stdcall Factorial(unsigned int n) {
    assert(n > 0);
    unsigned long long result = 1;
    for (unsigned int i = n; i >= 1; --i) result *= i;
    return result;
}
