#pragma once
#ifndef __BITOPS_H__
    #define __BITOPS_H__
    #include <stdbool.h>
    #include <stdint.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      CAUTION                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// offset argument in bit manipulation functions means the literal offset into the stream of bits                    //
// E.G AN OFFSET 3 MEANS THE FOURTH BIT OF THE FIRST BYTE NOT THE BIT AT 2^3 VALUE POSITION OF THE FIRST BYTE!       //
// IN BYTE 0b0100'1000, OFFSET 3 POINTS TO THE BIT INSIDE THE PARENTHESIS 0b010(0)'1000                              //
// NOT THE BIT AT THE POSITION OF 2^3 WHICH IS 0b0100'(1)000                                                         //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// returns the nth bit in the buffer (which is an array of bytes that is viewed as a contiguous stream of bits)
static __forceinline bool __stdcall getbit(
    _In_ register const uint8_t* const restrict bitstream, _In_ register const size_t offset /* nth bit */
) {
    // const uint8_t byte   = bitstream[offset / 8 /* deliberate integer division */]; // first find the byte that contains the asked bit.
    // const size_t  bit    = offset % 8;                                              // offset of the asked bit within the byte.
    // uint8_t       mask   = 0b1000'0000;
    // for (size_t _ = 0; _ < bit; ++_) mask >>= 1;
    // mask >>= bit;

    return bitstream[offset / 8] & (0b1000'0000 >> (offset % 8));
}

// toggles a select bit on or off.
static __forceinline void __stdcall setbit(
    _Inout_ register uint8_t* const restrict bitstream, _In_ const size_t offset, _In_ register const bool flag /* on or off */
) {
    // const size_t bit    = offset % 8;
    // uint8_t      mask   = 0b1000'0000;
    // for (size_t _ = 0; _ < bit; ++_) mask >>= 1;
    // mask              >>= bit;

    if (flag)
        bitstream[offset / 8] |= 0b1000'0000 >> (offset % 8);
    else
        bitstream[offset / 8] &= ~(0b1000'0000 >> (offset % 8));
}

// computes the bitwise xor of the passed buffers, and stores the result in the output buffer.
static __forceinline void __stdcall xorbit( // the result will be false only if both of the bits are identical
    _In_ register const uint8_t* const restrict ibuff_a,
    _In_ register const uint8_t* const restrict ibuff_b,
    _Inout_ register uint8_t* const restrict obuff,
    _In_ register const size_t offset
) {
    getbit(ibuff_a, offset) == getbit(ibuff_b, offset) ? setbit(obuff, offset, false) : setbit(obuff, offset, true);
}

#endif // !__BITOPS_H__
