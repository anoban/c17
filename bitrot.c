#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// get's the nth BIT in the buffer (which is an array of bytes)
// views bitstream as a contiguous stream of bits.
static inline bool __stdcall getbit(_In_ const uint8_t* const restrict bitstream, _In_ const size_t offset /* nth bit */) {
    const uint8_t byte   = bitstream[offset / 8]; // first find the byte that contains the asked bit.
    const size_t  offbit = offset % 8;            // offset of the asked bit within the byte.
    uint8_t       mask   = 0x01;                  // 0000 0001
    for (size_t i = offbit; i > 0; --i) mask <<= 1;
    return byte & mask;
}

// switches a select bit on or off.
static inline void __stdcall setbit(
    _Inout_ uint8_t* const restrict bitstream, _In_ const size_t offset, _In_ const bool flag /* on or off */
) {
    const size_t offbit = offset % 8;
    uint8_t      mask   = 0x01;                                                               // 0000 0001
    for (size_t i = offbit; i > 0; --i) mask <<= 1;
    if (flag)
        bitstream[offset / 8] |= mask;
    else
        bitstream[offset / 8] &= (~mask);
    return;
}

// computes the bitwise xor of the passed buffers, and stores the result in the output buffer.
static inline void __stdcall xorbit(
    _In_ const uint8_t* const restrict inbuff_0,
    _In_ const uint8_t* const restrict inbuff_1,
    _Inout_ uint8_t* const restrict outbuff,
    _In_ const size_t offset
) {
    if (getbit(inbuff_0, offset) != getbit(inbuff_1, offset))
        setbit(outbuff, offset, true);
    else
        setbit(outbuff, offset, false);
    return;
}

// quite similar to bit shifts but here the bit that gets pushed off the boundary will be reintroduced into the byte at the other end.
// 10110110 left rotated by 4 bits will be        01101011
// whereas left shifting by 4 bits will result in 00001011
static inline void __stdcall leftrotbit(
    _Inout_ uint8_t* const restrict bitstream,
    _In_ const size_t length /* number of bits in the buffer */,
    _In_ const size_t n /* rotate n bits left */
) {
    /*
    the goal is to rotate the bitstream left by n bits, starting from the offset bit.
    first stores the leftmost bit of the buffer temporarily, and shifts all the bits left and assigns the temporarily stored bit to the
    rightmost position. Iteratively repeats this n times, admittedly inefficient, an efficient implementation would likely call for an
    assembly routine or intrinsics using SIMD instructions.
    */

    if (!length) return;

    bool leftmost = false,
         pushoff  = false; // leftmost and rightmost bits within the shift boundary i.e bitstream + offset to bitstream + offset + n

    for (size_t _ = 0; _ < n; ++_) {                        // number of bits to rotate
        for (size_t j = 0; j <= (length - 1) / 8LLU; ++j) { // number of bytes in the specified bitstream
            // captures the state of the first bit of the selected byte
            pushoff = getbit(bitstream + j, 0 /* first bit of the current byte */);
            // wprintf_s(L"First bit if %zu th byte is %d\n", j, pushoff);

            if (!j) // first bit and last bit of the stream need swapping, so store the first bit of first byte.
                leftmost = pushoff;
            else
                // swap the first (leftmost) bit of the current byte with the last (rightmost) bit of the previous byte.
                setbit(bitstream + j - 1 /* picking the previous byte */, 7 /* the last bit */, pushoff);
            bitstream[j] >>= 1; // Intel x86_64 is little endian, so we need to do a right shift for left rotation
        }
        // set the last (rightmost) bit of the rotate window to the first bit of it.
        setbit(bitstream, length - 1, leftmost);
    }

    return;
}

int main(void) {
    const uint8_t refbits[] = { 255, 0, 0, 1 };
    uint8_t       bits[]    = { 255, 0, 0, 1 };
    // for (size_t i = 0; i < __crt_countof(bits) * 8; ++i) wprintf_s(L"Position: %4zu, state: %2d\n", i, getbit(bits, i));

    // wprintf_s(L"Position: %4zu, state: %2d\n", i, getbit(bits, i));
    // setbit(bits, i, false);
    leftrotbit(bits, sizeof(bits) * 8, 20);
    // wprintf_s(L"Position: %4zu, state: %2d\n", i, getbit(bits, i));

    for (size_t i = 0; i < __crt_countof(bits) * 8; ++i) wprintf_s(L"%4zu: %2d %2d\n", i, getbit(refbits, i), getbit(bits, i));
    return EXIT_SUCCESS;
}
