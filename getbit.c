#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// clang .\getbit.c -Wall -O3 -std=c17 -Wextra -Wpedantic -o .\getbit.exe

static inline bool getbit(_Inout_ const uint8_t* const restrict bitstream, _In_ const size_t offset /* nth bit */) {
    const uint8_t byte   = bitstream[offset / 8]; // first find the byte that contains the asked bit.
    const size_t  offbit = offset % 8;            // offset of the asked bit within the byte.
    uint8_t       mask   = 0x01;                  // 0000 0001
    for (size_t i = offbit; i > 0; --i) mask <<= 1;
    return byte & mask;
}

static inline void setbit(_Inout_ uint8_t* const restrict bitstream, _In_ const size_t offset, _In_ const bool flag) {
    const size_t offbit = offset % 8;
    uint8_t      mask   = 0x01; // 0000 0001
    for (size_t i = offbit; i > 0; --i) mask <<= 1;
    if (flag)
        bitstream[offset / 8] |= mask;
    else
        bitstream[offset / 8] &= (~mask);
    return;
}

int main(void) {
    const uint8_t bytes[]  = { 0, 2, 4, 8, 16, 32, 64, 128, 255 };
    uint8_t       _bytes[] = { 0, 2, 4, 8, 16, 32, 64, 128, 255 };
    for (size_t i = 0; i < (sizeof(bytes)) * 8; ++i) wprintf_s(L"Byte: %u, Bit: %zu, State: %d\n", i / 8, i, getbit(bytes, i));
    _putws(L"\n");

    for (size_t i = 0; i < (sizeof(bytes)) * 8; ++i) {
        setbit(_bytes, i, false);
        wprintf_s(L"Byte: %u, Bit: %zu, State: %d\n", i / 8, i, getbit(_bytes, i));
    }
    _putws(L"\n");

    for (size_t i = 0; i < (sizeof(bytes)) * 8; ++i) {
        setbit(_bytes, i, true);
        wprintf_s(L"Byte: %u, Bit: %zu, State: %d\n", i / 8, i, getbit(_bytes, i));
    }
    _putws(L"\n");

    return 0;
}
