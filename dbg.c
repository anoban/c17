// practice using WinDbg

#include <stdint.h>

int main(void) {
    const uint64_t x   = 1997;
    uint64_t       sum = 0;
    for (size_t i = 0; i < UINT8_MAX; ++i) sum += i;
    return 0;
}