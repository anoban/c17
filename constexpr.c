#include <stdio.h>
#include <stdlib.h>

#ifndef __GNUC__
    #error As of 06-08-2024 only GCC supports C23 constexpr object initialization
#endif

typedef struct data {
        long double        x;
        char               y;
        unsigned long long z;
} data_t;

int main(void) {
    [[maybe_unused]] constexpr data_t my_data = { .x = 12.00, .y = 'A', .z = 76'876'498'236'498 };
    printf_s("%Lf, %c, %llu\n", my_data.x, my_data.y, my_data.z);
    return EXIT_SUCCESS;
}
