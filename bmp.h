#pragma once
#ifndef __BMP__
    #define __BMP__
    #include <stdint.h>

// a header containing function prototypes and data structure definitions for bmp.c

// user defined datatypes that conflict with wingdi.h data type are prefixed with a double underscore to avoid conflicts.
// almost every functionality related to BMPs seems to be abailable in wingdi.h yikes!
// Reinventing the wheel with 0 regrets :)

// .bmp is one of the simplest image encoding formats!
// so it'd help a lot to start with simple decoders and then move on to more complex ones.

// Windows .bmp images can be compressed, but is only valuable when the image has large blocks with identical colours.
// that severely restrticts the usefulness of compression when it comes to .bmp images.

// .bmp file contents
// 1) file header
// 2) image header
// 3) color table
// 4) pixel data

// every Windows BMP begins with a BITMAPFILEHEADER struct
// this helps in recognizing the file format as .bmp
// the first two bytes will be 'B', 'M'

static const uint16_t BMPSOI_LE = ((uint16_t) 'M') << 8 | (uint16_t) 'B';

typedef struct {
        uint16_t SOI; // BM
        uint32_t FSIZE;
        uint32_t RESERVED; // this is actually two consecutive 16 bit elements, but who cares :)
        uint32_t PIXELDATASTART;
} __BITMAPFILEHEADER;

// image header comes in two variants!
// one representing OS/2 BMP format (BITMAPCOREHEADER) and another representing the most common Windows BMP format.
// (BITMAPINFOHEADER)
// however there are no markers to identify the type of the image header present in the bmp image.
// the only way to determine this is to examine the struct's size filed, that is the first 4 bytes of the struct.
// sizeof BITMAPCOREHEADER is 12 bytes
// sizeof BITMAPINFOHEADER is >= 40 bytes.
// from Windows 95 onwards, Windows supports an extended version of BITMAPINFOHEADER, which could be larger than 40 bytes!

// types of compressions used in BMP files.
typedef enum { RGB, RLE8, RLE4, BITFIELDS } BMPCOMPRESSIONKIND;

typedef struct {
        uint32_t HEADERSIZE; // >= 40 bytes.
        uint32_t WIDTH;
        int32_t
            HEIGHT; // usually an unsigned value, a negative value alludes that the pixel data is ordered top down, instead of the customary
        // bottom up order. bmp images with a - height values may not be compressed!
        uint16_t           NPLANES;       // must be 1
        uint16_t           NBITSPERPIXEL; // 1, 4, 8, 16, 24 or 32
        BMPCOMPRESSIONKIND CMPTYPE;
        uint32_t           IMAGESIZE;    // 0 if not compressed.
        uint32_t           RESPPMX;      // resolution in pixels per meter along x axis.
        uint32_t           RESPPMY;      // resolution in pixels per meter along y axis.
        uint32_t           NCMAPENTRIES; // number of entries in the colourmap that are used.
        uint32_t           NIMPCOLORS;   // number of important colors.
} __BITMAPINFOHEADER;

// a BMP with BITMAPCOREHEADER cannot be compressed.
// and is quite rare to come across.
typedef struct {
        uint32_t HEADERSIZE; // 12 bytes
        uint16_t WIDTH;
        uint16_t HEIGHT;
        uint16_t NPLANES;       // must be 1
        uint16_t NBITSPERPIXEL; // 1, 4, 8 or 24
} __BITMAPCOREHEADER;

typedef enum { TOPDOWN, BOTTOMUP } BMPPIXDATAORDERING;

// a colour palette structure immediately follows the BMP file header
// "colour palette" is a fancy term for the pixel mapping in binary form in the BMP file.
// each consecutive 32 bits can be dereferenced as a RGBQUAD structure.
// there are 3 variants of the colour palette structure.
// first two variants are used to map pixel data to RGB values, when the bit count is 1, 4 or 8.
// these two variants are common in Windows BMP files.
typedef struct {
        uint8_t BLUE;
        uint8_t GREEN;
        uint8_t RED;
        uint8_t RESERVED; // must be 0, but seems to be 0xFF in most BMPs, yikes!
} ___RGBQUAD;

// BMP files in OS/2 use the third variant
typedef struct {
        uint8_t BLUE;
        uint8_t GREEN;
        uint8_t RED;
} ___RGBTRIPLE;

// en enum to specify the RGB -> BW conversion method.
// AVERAGE takes the mean of R, G and B values.
// WEIGHTED_AVERAGE does GREY = (0.299 R) + (0.587 G) + (0.114 B)
// LUMINOSITY does LUM = (0.2126 R) + (0.7152 G) + (0.0722 B)
typedef enum { AVERAGE, WEIGHTED_AVERAGE, LUMINOSITY } RGBTOBWKIND;

// the color palette in pixel buffer is in BGR order NOT RGB!
typedef enum { REMRED = 0xFFFFF, REMGREEN, REMBLUE, REMRG, REMRB, REMGB } RMCOLOR;

#endif // !__BMP__