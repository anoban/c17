#pragma once
#ifndef __PNG__
    #define __PNG__
    #include <master.h>

// By default PNG uses big-endian representation for multibyte data.
// PNG files are made of "chunks"
// a PNG file may contain private chunks, encoded by particular image processing applications for their
// own use. These chunks can be safely ignored by generic decoders.

// each chunks is given a 4 byte ASCII name.
// the first, second and the fourth characters of this name can be either upper or lower case.
// decoders can discern a lot of information about the file, based purely on this name.

typedef struct {
        uint32_t LENGTH;  // number of bytes in the DATA field.
        char     NAME[5]; // name of the chunk (last byte for the null terminator, for convenience.)
        uint8_t* DATA;    // chunk data.
        uint32_t CRC32;   // a 32 bit Cyclic Redundancy Check (CRC) value calculated from the data.
} PNG_CHUNK_HEAD;

// Critical chunks.
// these are chunks that every decoder must absolutely process in order to decode the image.
// PNG standard defines 4 critical chunks,
// "IHDR"
// "PLTE"
// "IDAT"
// "IEND"

// Public & private chunks.
// Second character in the name must be upper case for public chunks.
// all chunks defined by the PNG standard are public chunks.
// private chunks encoded by applications for their own use must have the second character in lower case.

// Safe to copy chunks.
// last (fourth) character in the name should be lower case if the chunk is safe to copy.
// chunks with a name that has an upper case fourth character should not attempt to copy that chunk,
// given it cannot recognize the given chunk with the available logic.

// Cyclic Redundancy Check (CRC)

// Each PNG chunk has a CRC32 value that has been calculated from the type code e.g. "IHDR" and the chunk data.
// CRC is a way to check the integrity of binary data, very commonly used in network transmissions.
// Before sending network packets, the server computes the CRC value of the data and appends it at the
// end of the data. Client is then expected to validate the integrity of the received packet by computing the
// CRC value and comparing that with the one sent by the server.

// PNG files contain CRC codes included in each chunks so that the decoders can validate that a given chunk has
// not suffered any corruption during storage or transmission.

// CRC function is based on doing modulo divisions by 3 (x % 2) on the input data.
// where each bit (NOT BYTE) in the input stream is treated as a coefficient in a large polynomial.
// CRC value is the remainder of the modulus division.

// In mathematics, a polynomial is an expression consisting of indeterminates (also called variables)
// and coefficients, that involves only the operations of addition, subtraction, multiplication, and
// positive-integer powers of variables.

// The polynomial used by PNG encoders is, (^ denotes power here)
// imagine X as an imaginary base value.
// X^32 + X^26 + X^23 + X^22 + X^16 + X^12 + X^11 + X^10 + X^8 + X^7 + X^5 + X^4 + X^2 + X^1 + X^0

// X^2 + X^1 + X^0 (implicitly states there's no X^3)				-> 0111		(bits 0 - 3)
// X^7 + X^5 + X^4 (implicitly states there's no X^6)				-> 1011		(bits 4 - 7)
// X^11 + X^10 + X^8 (implicitly states there's no X^9)				-> 1101		(bits 8 - 11)
// X^12 (implicitly states there's no X^13, X^14 & X^15)			-> 0001		(bits 12 - 15)
// X^16 (implicitly states there's no X^17, X^18 & X^19)			-> 0001		(bits 16 - 19)
// X^23 + X^22 (implicitly states there's no X^20 & X^21)			-> 1100		(bits 20 - 23)
// X^26 (implicitly states there's no X^24, X^25 & X^27)			-> 0100		(bits 24 - 27)
// X^32 (implicitly states there's no X^28, X^29, X^30 & X^31)		-> 0000		(bits 28 - 31)
// and the final byte is X^32										-> 0001		(bits 32 - 35)

// which essentially is 1 0000 0100 1100 0001 0001 1101 1011 0111 in binary.

// PNG decoders customarily use a pre-existing lookup table to find out CRC values for each byte.
// this table is basically an array of 256 unsigned 32 bit integers.

    #ifdef __RUNTIME_COMPUTE_CRC32__

// a reference implementation follows, (yes this is a header, but I don't plan to use this, so...)
// we need space for 256 values for all possible byte values, ranging from 0 to 255 (UCHAR_MAX)
uint32_t crc_table[UCHAR_MAX + 1] = { 0 };

void __forceinline __stdcall init_crc_table(void) {
    for (uint32_t i = 0; i <= UCHAR_MAX; ++i) { // each byte in the table.
        crc_table[i] = i;
        for (uint32_t j = 0; j < 8; ++j) { // each bit position in a byte

            // 0x1U is 0000 0000 0000 0000 0000 0000 0000 0001
            // if anything is to yield 0000 0000 0000 0000 0000 0000 0000 0000 with a bitwise and operation
            // that must have 0 as the last bit, otherwise the result could be 1 or more than 1.

            //   xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx
            // & 0000 0000 0000 0000 0000 0000 0000 0001
            // = 0000 0000 0000 0000 0000 0000 0000 0000

            // any combination of bits in any sequence can give a non-zero value in this bitwise and operation
            // given that its last bit is not zero.
            // to satisfy this if condition, the value in the CRC table must have 0 as the last bit.
            // any uint32_t satisfying this (xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxx0) criteria
            // will pass this conditional.

            if (!(crc_table[i] & 0x1U)) crc_table[i] >>= 1;

            // if the value has the last bit turned off, right shift the bits 8 times.
            // for example a value 1010 0010 1111 1010 1100 1010 1011 0010 (2,734,344,882) will become
            // 0000 0000 1010 0010 1111 1010 1100 1010 (10,681,034)

            else
                crc_table[i] = 0xEDB88320L ^ (crc_table[i] >> 1);

            // 0xEDB88320L is 1110 1101 1011 1000 1000 0011 0010 0000
            // if the last bit of the value is turned on, then right shift the bits by one
            // and then do a bitwise XOR with 1110 1101 1011 1000 1000 0011 0010 0000 and
            // assign the result to the table value & repeat this 8 times.

            // in XOR, if bits in a given position of the two operands are identical, the resulting bit at
            // the corresponding position will be turned off (0), otherwise it will be turned on (1)
            //	 0101 0010
            // ^ 1000 1010
            // = 1101 1000
        }
    }
    return;
}

    #endif // __RUNTIME_COMPUTE_CRC32__

// It's better to do the computation beforehand and just store the values, instead of doing the computations
// in real time. If C++, constexpr could be used, welp!

    #ifdef __CRC32_LOOKUP_NEEDED__

// checked correct :)
const uint32_t CRC_TABLE[] = {
    0,          1996959894, 3993919788, 2567524794, 124634137,  1886057615, 3915621685, 2657392035, 249268274,  2044508324, 3772115230,
    2547177864, 162941995,  2125561021, 3887607047, 2428444049, 498536548,  1789927666, 4089016648, 2227061214, 450548861,  1843258603,
    4107580753, 2211677639, 325883990,  1684777152, 4251122042, 2321926636, 335633487,  1661365465, 4195302755, 2366115317, 997073096,
    1281953886, 3579855332, 2724688242, 1006888145, 1258607687, 3524101629, 2768942443, 901097722,  1119000684, 3686517206, 2898065728,
    853044451,  1172266101, 3705015759, 2882616665, 651767980,  1373503546, 3369554304, 3218104598, 565507253,  1454621731, 3485111705,
    3099436303, 671266974,  1594198024, 3322730930, 2970347812, 795835527,  1483230225, 3244367275, 3060149565, 1994146192, 31158534,
    2563907772, 4023717930, 1907459465, 112637215,  2680153253, 3904427059, 2013776290, 251722036,  2517215374, 3775830040, 2137656763,
    141376813,  2439277719, 3865271297, 1802195444, 476864866,  2238001368, 4066508878, 1812370925, 453092731,  2181625025, 4111451223,
    1706088902, 314042704,  2344532202, 4240017532, 1658658271, 366619977,  2362670323, 4224994405, 1303535960, 984961486,  2747007092,
    3569037538, 1256170817, 1037604311, 2765210733, 3554079995, 1131014506, 879679996,  2909243462, 3663771856, 1141124467, 855842277,
    2852801631, 3708648649, 1342533948, 654459306,  3188396048, 3373015174, 1466479909, 544179635,  3110523913, 3462522015, 1591671054,
    702138776,  2966460450, 3352799412, 1504918807, 783551873,  3082640443, 3233442989, 3988292384, 2596254646, 62317068,   1957810842,
    3939845945, 2647816111, 81470997,   1943803523, 3814918930, 2489596804, 225274430,  2053790376, 3826175755, 2466906013, 167816743,
    2097651377, 4027552580, 2265490386, 503444072,  1762050814, 4150417245, 2154129355, 426522225,  1852507879, 4275313526, 2312317920,
    282753626,  1742555852, 4189708143, 2394877945, 397917763,  1622183637, 3604390888, 2714866558, 953729732,  1340076626, 3518719985,
    2797360999, 1068828381, 1219638859, 3624741850, 2936675148, 906185462,  1090812512, 3747672003, 2825379669, 829329135,  1181335161,
    3412177804, 3160834842, 628085408,  1382605366, 3423369109, 3138078467, 570562233,  1426400815, 3317316542, 2998733608, 733239954,
    1555261956, 3268935591, 3050360625, 752459403,  1541320221, 2607071920, 3965973030, 1969922972, 40735498,   2617837225, 3943577151,
    1913087877, 83908371,   2512341634, 3803740692, 2075208622, 213261112,  2463272603, 3855990285, 2094854071, 198958881,  2262029012,
    4057260610, 1759359992, 534414190,  2176718541, 4139329115, 1873836001, 414664567,  2282248934, 4279200368, 1711684554, 285281116,
    2405801727, 4167216745, 1634467795, 376229701,  2685067896, 3608007406, 1308918612, 956543938,  2808555105, 3495958263, 1231636301,
    1047427035, 2932959818, 3654703836, 1088359270, 936918000,  2847714899, 3736837829, 1202900863, 817233897,  3183342108, 3401237130,
    1404277552, 615818150,  3134207493, 3453421203, 1423857449, 601450431,  3009837614, 3294710456, 1567103746, 711928724,  3020668471,
    3272380065, 1510334235, 755167117
};

    #endif // __CRC32_LOOKUP_NEEDED__

// PNG Signature.

// The byte stream of a binary PNG file must start with a 8 byte PNG signature.
// with the following values: 137, 80, 78, 71, 13, 10, 26 and 10.
// These are 137, 'P', 'N', 'G', '\r', '\n', CRTL + Z, '\r' in ASCII

// This signature has another role in validating the image integrity.
// Text files in MS-DOS / Windows systems use '\r' followed by a '\n' to mark line ends.
// In Unix systems this is marked with '\n'.
// When passing files between these systems, in TEXT mode, these line ends will be converted accoringly
// during the transaction. e.g. In a Windows to Linux transfer '\r''\n' sequences will be replaced with
// plain '\n' characters.
// Has the png been transferred using text mode in either of the directions, at least one of these '\r''\n'
// or '\n' will be altered in the PNG signature. So a decoder doesn't need to spend time doing CRC32 validations
// on chunk data, when it already knows that the file has been mishandled.

// Colour representation in PNGs.

// PNG supports 5 different colour formats for representing pixels.
// This format of choice is registered in the IHDR chunk.

// 1) RGB triples
// Similar to BMPs, each pixel is represented by three values (either 8 bits or 16 bits)
// In contrast to BMP which stores these values in BGR order, PNG stores them in the RGB order.
// RGB triples may only be used when the bit depth is 8 or 16 bits.

// 2) Palettes.
// PNGs can use colour palettes, and the size of the palette depends on the sample precision.
// PNGs that use a palette must define it in the PLTE chunk.
// Palettes can only be used when the bit depth is 1, 2, 4 or 8 bits.

// 3) Greyscale.
// In greyscale type there's one component per image, representing a pixel.
// This type can be used with PNGs with all bit depths.
// In most systems, the PNG decoder will need to inflate the greyscale palette to RGB for rendering & manipulation.
// It basically needs to create RGB palettes for each greyscale pixel, essentially copying the greyscale value
// to the R, G & B components of the RGB palatte

// 4) RGB with alpha channel.
// PNGs support alpha channels to control the transparency of images.
// Alpha channel allows the image to be combined with its background. In this palette, each pixel has an
// additional alpha value, whose bit count is same as the image's bit depth.
// An alpha value of 0 implies that the pixel is completely transparent.
// When the alpha value is 2 ^ (bit depth - 1), the pixel is completely covered by the image. (^ - power)
// When the alpha value is in-between these two values, the pixel is manipulated as follows,
// alpha channel merging ->
// output.R = (alpha * image.R + (MAXPIXELVALUE - alpha) * background.R) / MAXPIXELVALUE
// output.G = (alpha * image.G + (MAXPIXELVALUE - alpha) * background.G) / MAXPIXELVALUE
// output.B = (alpha * image.B + (MAXPIXELVALUE - alpha) * background.B) / MAXPIXELVALUE
// e.g. If the bit depth is 16 bits, MAXPIXELVALUE will be 2 ^ (16 - 1)

// 5) Greysacle with alpha channel.
// Alpha channels can also be used with greyscale images, but the bit depth is restricted to 8 or 16.
// Each pixel following this encoding is represented using two subsequent identically sized values.
// Alpha value following the pixel intensity value.
// alpha channel merging & inflation -> (.P refers to pixel intensity)
// output.R = (alpha * image.P + (MAXPIXELVALUE - alpha) * background.P) / MAXPIXELVALUE
// output.G = (alpha * image.P + (MAXPIXELVALUE - alpha) * background.P) / MAXPIXELVALUE
// output.B = (alpha * image.P + (MAXPIXELVALUE - alpha) * background.P) / MAXPIXELVALUE

// Chunk headers and chunk data.

// 1) IHDR
// The size of the data block of the IHDR chunk must always be 13 bytes. Anything else is invalid.

// There seems to be a struct typedef RGBTRIPLE defined in Wingdi.h, welp.
// Which messes with this, so prefixing everything with IHDRCT
typedef enum { IHDRCT_GREYSCALE, IHDRCT_RGBTRIPLE = 2, IHDRCT_PALETTE, IHDRCT_GREYSCALE_ALPHA, IHDRCT_RGB_ALPHA = 6 } IHDR_COLORTYPE;

typedef struct {
        uint32_t       WIDTH;              // Image width in pixels.
        uint32_t       HEIGHT;             // Image height in pixels.
        uint8_t        BITDEPTH;           // Can be 1, 2, 4, 8 or 16.
        IHDR_COLORTYPE COLORTYPE;          // 1 byte
        uint8_t        COMPRESSION_METHOD; // Must be 0.
        uint8_t        FILTER_METHOD;      // Must be 0.
        uint8_t        INTERLACE_METHOD;   // 0 if not interlaced. 1 if Adam 7 interlaced.
                                           // Interlacing is a way to encode multiple images in a PNG file, like a GIF file.
                                           // Not planning to accomodate this.
} IHDR_DATA;

// prototypes.

bool validate_png_signature(_In_ const uint8_t* restrict png_stream, _In_ const uint64_t fsize);

PNG_CHUNK_HEAD extract_IHDR(_In_ const uint8_t* restrict png_stream, _In_ const uint64_t fsize);

IHDR_DATA extract_IHDR_data(_In_ const PNG_CHUNK_HEAD ihdr_head);

void print_IHDR_info(_In_ const PNG_CHUNK_HEAD ihdr_head, _In_ const IHDR_DATA ihdr_data);
#endif // ! __PNG__