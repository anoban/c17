#include <stdint.h>
#include <stdio.h>

int main(void) {
    int32_t x = 56;
    ++x       = 12; // will work just fine in C++, Error in C
    // error: expression is not assignable
    wprintf_s(L"%d\n", x);
    --x = 0; // error: expression is not assignable
    wprintf_s(L"%d\n", x);

    x-- = 12; // error: expression is not assignable
    x++ = 1;  // error: expression is not assignable

    return 0;
}
