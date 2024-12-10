#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>

int wmain(void) {
#if defined(__llvm__) && defined(__clang__)
    static const __m128i BYTE_REVERSE_MASK = { 0x0C0D0E0F08090A0B, 0x0405060700010203 };
#elif defined(_MSC_VER) && defined(_MSC_FULL_VER)
    static const __m128i BYTE_REVERSE_MASK = {
        // !!! WE WANT TO REVERSE THE PIXELS NOT THE BYTES, REVERSING PIXELS MEANS REVERSING CONSECUTIVE 4 BYTE BLOCKS
        .m128i_u8 = { 12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3 }
    };
#endif

    __m128i       reversed;
    unsigned char bytes[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    reversed              = _mm_shuffle_epi8(_mm_loadu_epi8(bytes), BYTE_REVERSE_MASK);

    for (unsigned i = 0; i < sizeof(__m128i); ++i) wprintf_s(L"%4d\n", reversed.m128i_u8[i]);

    return EXIT_SUCCESS;
}
