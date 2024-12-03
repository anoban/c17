#include <immintrin.h>
#include <intrin.h> // SSSE3 _mm_shuffle_pi8()
#include <stdio.h>
#include <stdlib.h>

static inline unsigned long long ullong_from_be_bytes(_In_reads_bytes_(8) const unsigned char* const restrict bytestream) {
    static const __m64 mask = { 0b0000000'00000001'00000010'00000011'00000100'00000101'00000110'00000111 };
    return _mm_shuffle_pi8(*(__m64*) bytestream, mask);
}

static inline unsigned long ulong_from_be_bytes(_In_reads_bytes_(4) const unsigned char* const restrict bytestream) {
    static const __m64 mask = { 0b00000100'00000101'00000110'00000111'0000000'00000001'00000010'00000011 };
    return (unsigned long long) _mm_shuffle_pi8(*(__m64*) bytestream, mask);
}

static const unsigned char bytes[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

int                        wmain(void) {
    wprintf_s(L"LE :: %llX\n", *(unsigned long long*) bytes);
    wprintf_s(L"BE :: %llX\n", ullong_from_be_bytes(bytes));

    wprintf_s(L"LE :: %lX\n", *(unsigned long*) bytes);
    wprintf_s(L"BE :: %lX\n", ulong_from_be_bytes(bytes));

    return EXIT_SUCCESS;
}
