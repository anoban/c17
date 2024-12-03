#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t crc32_table[256];

void            build_crc32_table(void) {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t ch  = i;
        uint32_t crc = 0;
        for (size_t j = 0; j < 8; j++) {
            uint32_t b   = (ch ^ crc) & 1;
            crc        >>= 1;
            if (b) crc = crc ^ 0xEDB88320;
            ch >>= 1;
        }
        crc32_table[i] = crc;
    }
}

int main(void) {
    build_crc32_table();
    for (size_t i = 0; i < 256; ++i) wprintf_s(L"%08X,\n", crc32_table[i]);
    return 0;
}
