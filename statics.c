#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// static inline uint64_t array_sum(uint64_t array[static 10], uint64_t length) {
//     uint64_t sum = 0;
//     for (uint64_t i = 0; i < length; ++i) sum += array[i];
//     return sum;
// }

uint64_t static inline std_compliant_array_sum(uint64_t array[10], uint64_t length) {
    uint64_t sum = 0;
    for (uint64_t i = 0; i < length; ++i) sum += array[i];
    return sum;
}

int main(void) {
    uint64_t numbers[]   = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    uint64_t __numbers[] = { 1, 2, 5, 6, 7, 8, 9, 10, 11 };
    uint64_t sum         = std_compliant_array_sum(numbers, 15);
    // array length is not >= 10 :)
    // icx issues a warning, WOW!
    // MSVC refuses to compile the source :&(
    // MISRA C:2012, 17.6 - The declaration of an array parameter shall not contain the static keyword between the []
    // Ooopsies!
    // standard compliant solution is to define the array with a size!
    // static inline uint64_t array_sum(uint64_t array[10], uint64_t length)
    uint64_t __sum       = std_compliant_array_sum(__numbers, 9);
    printf_s("Sum is %llu\n", sum);
    printf_s("Sum is %llu\n", __sum);
    exit(0);
}
