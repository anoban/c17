#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

[[nodiscard]] uint64_t sum(uint64_t* restrict array, const uint64_t length) {
    uint64_t sum = 0;
    for (size_t i = 0; i < length; ++i) sum += array[i];
    return sum;
}

int main(void) {
    uint64_t array[1000] = { 0 };
    for (size_t i = 0; i < 1000; ++i) array[i] = rand();
    sum(array, 100);

    uint64_t user = 0;
    puts("Enter an integer between 1 and 3!");
    scanf_s("%llu", &user);

    switch (user) {
        case 1  : puts("That's one!"); [[fallthrough]];
        case 2  : puts("That's two!"); break;
        case 3  : puts("That's three!");
        default : puts("Finally huh?");
    }

    return EXIT_SUCCESS;
}