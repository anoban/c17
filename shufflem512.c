#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>

static const __m512i BYTE_REVERSE_MASK = {
    .m512i_u8 = { 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42,
                 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20,
                 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,  1,  0 }
};

int wmain(void) {
    static const long long REVERSE = 0x0001020304050607;
    __m512i                mask    = _mm512_set1_epi64(REVERSE);
    for (unsigned i = 0; i < sizeof(__m512i); ++i) wprintf_s(L"%4d\n", mask.m512i_u8[i]);
    _putws(L"\n");

    __m512i       reversed;
    unsigned char bytes[] = { 0,  2,  4,  6,  8,  10, 12,  14,  16,  18,  20,  22,  24,  26,  28,  30,  32,  34,  36,  38, 40, 42,
                              44, 46, 48, 50, 52, 54, 56,  58,  60,  62,  64,  66,  68,  70,  72,  74,  76,  78,  80,  82, 84, 86,
                              88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126 };

    reversed              = _mm512_shuffle_epi8(_mm512_loadu_epi8(bytes), BYTE_REVERSE_MASK);

    for (unsigned i = 0; i < sizeof(__m512i); ++i) wprintf_s(L"%4d\n", reversed.m512i_u8[i]);

    return EXIT_SUCCESS;
}
