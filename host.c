#include <stdio.h>

#include "dynlib.h"

int main(void) {
    int32_t squares[] = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100 };
    int64_t sum       = summer(squares, 11);
    printf_s("Sum is %lld\n", sum);
    return 0;
}
