#include <limits.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>

// love C man :))))

int main(void) {
    alignas(2) uint8_t bytes[]  = { 0xFF, 0xFF };
    uint16_t           value    = 0x0000;

    // "Type safety" my ass :)
    value                      |= *((uint16_t*) bytes);

    if (value == UINT16_MAX) puts("woohoo");
    printf_s("value: %X, UINT16_MAX: %X\n", value, UINT16_MAX);
    return 0;
}
