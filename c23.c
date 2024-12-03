#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STR(expression)     #expression
#define WEXPAND(expression) (L"" STR(expression)) // this indirection is needed to enable the expansion of expression macro

[[nodiscard]] uint64_t sum(uint64_t* restrict array, const uint64_t length) {
    uint64_t sum = 0;
    for (size_t i = 0; i < length; ++i) sum += array[i];
    return sum;
}

int main(void) {
    srand(time(nullptr));
    uint64_t array[1000] = { 0 };
    for (size_t i = 0; i < 1000; ++i) array[i] = rand();
    [[maybe_unused]] const uint64_t _sum = sum(array, 100);

    const uint64_t                  user = rand() % 4;

    switch (user) {
        case 1  : puts("That's one!"); [[fallthrough]];
        case 2  : puts("That's two!"); break;
        case 3  : puts("That's three!");
        default : puts("Finally huh?");
    }
    _putws(WEXPAND(__STDC_VERSION__));
    return EXIT_SUCCESS;
}
