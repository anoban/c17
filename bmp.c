#pragma warning(disable : 4710)

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    #define _AMD64_ // architecture
    #define WIN32_LEAN_AND_MEAN
    #define WIN32_EXTRA_MEAN
#endif

#include <errhandlingapi.h>
#include <fileapi.h>
#include <handleapi.h>
#include <sal.h>

// user defined datatypes that conflict with wingdi.h data type are prefixed with a double underscore to avoid conflicts.

uint8_t static inline* open_image(const _In_ wchar_t* restrict file_name, _Out_ uint64_t* nread_bytes) {
    *nread_bytes    = 0;
    void *   handle = NULL, *buffer = NULL;
    uint32_t nbytes = 0;

    handle          = CreateFileW(file_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

    if (handle != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER file_size;
        if (!GetFileSizeEx(handle, &file_size)) {
            fprintf_s(stderr, "Error %lu in GetFileSizeEx\n", GetLastError());
            return NULL;
        }

        // add an extra megabyte to the buffer, for safety.
        size_t buffsize = file_size.QuadPart + (1024U * 1024);

        // caller is responsible for freeing this buffer.
        buffer          = malloc(buffsize);
        if (buffer) {
            if (ReadFile(handle, buffer, buffsize, &nbytes, NULL)) {
                *nread_bytes = nbytes;
                return buffer;
            } else {
                fprintf_s(stderr, "Error %lu in ReadFile\n", GetLastError());
                CloseHandle(handle);
                free(buffer);
                return NULL;
            }
        } else {
            fputs("Memory allocation error: malloc returned NULL", stderr);
            CloseHandle(handle);
            return NULL;
        }
    } else {
        fprintf_s(stderr, "Error %lu in CreateFileW\n", GetLastError());
        return NULL;
    }
}

bool static inline write_image(_In_ uint8_t* restrict buffer, const _In_ uint64_t buffsize, const _In_ wchar_t* restrict file_name) {
    HANDLE hfile = CreateFileW(file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hfile == INVALID_HANDLE_VALUE) {
        fprintf_s(stderr, "Error %lu in CreateFileW\n", GetLastError());
        return false;
    }

    uint32_t nbyteswritten = 0;
    if (!WriteFile(hfile, buffer, buffsize, &nbyteswritten, NULL)) {
        fprintf_s(stderr, "Error %lu in WriteFile\n", GetLastError());
        CloseHandle(hfile);
        return false;
    }
    return true;
}

// almost every functionality related to BMPs seems to be abailable in wingdi.h yikes!
// Reinventing the wheel with 0 regrets :)

// .bmp is one of the simplest image encoding formats!
// so it'd help a lot to start with simple decoders and then move on to more complex ones.

// Windows .bmp images can be compressed, but is only valuable when the image has large blocks with identical colours.
// that severely restrticts the usefulness of compression when it comes to .bmp images.

// .bmp file contents

// file header
// image header
// color table
// pixel data

// every Windows BMP begins with a BITMAPFILEHEADER struct
// this helps in recognizing the file format as .bmp

// the first two bytes will be 'B', 'M'

const uint16_t static SOBMP = ((uint16_t) 'M' << 8) | (uint16_t) 'B';

typedef struct {
        uint16_t SOI;      // BM
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
        uint32_t           IMAGESIZE;     // 0 if not compressed.
        uint32_t           RESPPMX;       // resolution in pixels per meter along x axis.
        uint32_t           RESPPMY;       // resolution in pixels per meter along y axis.
        uint32_t           NCMAPENTRIES;  // number of entries in the colourmap that are used.
        uint32_t           NIMPCOLORS;    // number of important colors.
} __BITMAPINFOHEADER;

// a BMP with BITMAPCOREHEADER cannot be compressed.
typedef struct {
        uint32_t HEADERSIZE;    // 12 bytes
        uint16_t WIDTH;
        uint16_t HEIGHT;
        uint16_t NPLANES;       // must be 1
        uint16_t NBITSPERPIXEL; // 1, 4, 8 or 24
} __BITMAPCOREHEADER;

// a colour palette structure immediately follows the BMP file header
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

// In Windows BMP formats the colour palette section contains 2^NBITSPERPIXEL number of ___RGBQUAD structures.
static __forceinline uint64_t __stdcall get_nrgbquads_in_cpalette(const _In_ __BITMAPINFOHEADER bmpinfh) {
    return (uint64_t) powl(2.0000L, (long double) bmpinfh.NBITSPERPIXEL);
}

static __forceinline BMPCOMPRESSIONKIND __stdcall get_bmp_compression_kind(const _In_ uint32_t cmpkind) {
    switch (cmpkind) {
        case 0 : return RGB;
        case 1 : return RLE8;
        case 2 : return RLE4;
        case 3 : return BITFIELDS;
    }
    return -1;
}

static inline __BITMAPFILEHEADER parse_bitmapfile_header(const _In_ uint8_t* restrict imstream, const _In_ uint64_t fsize) {
    assert(fsize >= sizeof(__BITMAPFILEHEADER));
    __BITMAPFILEHEADER header = { 0, 0, 0, 0 };
    // due to little endianness, two serial bytes 0x42, 0x4D will be interpreted as 0x4D42 when casted as
    // an uint16_t yikes!, thereby warranting a little bitshift.
    header.SOI                = (((uint16_t) (*(imstream + 1))) << 8) | ((uint16_t) (*imstream));
    if (header.SOI != SOBMP) {
        fputs("Error in parse_bitmapfile_header, file appears not to be a Windows BMP file\n", stderr);
        return header;
    }
    header.FSIZE          = *((uint32_t*) (imstream + 2));
    header.PIXELDATASTART = *((uint32_t*) (imstream + 10));
    return header;
}

static inline __BITMAPINFOHEADER parse_bitmapinfo_header(const _In_ uint8_t* restrict imstream, const _In_ uint64_t fsize) {
    assert(fsize >= (sizeof(__BITMAPFILEHEADER) + sizeof(__BITMAPINFOHEADER)));
    __BITMAPINFOHEADER header = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    if (*((uint32_t*) (imstream + 14U)) > 40) {
        fputs("BITMAPINFOHEADER larger than 40 bytes! BMP image seems to contain an unparsable file info header", stderr);
        return header;
    }
    header.HEADERSIZE    = *((uint32_t*) (imstream + 14U));
    header.WIDTH         = *((uint32_t*) (imstream + 18U));
    header.HEIGHT        = *((uint32_t*) (imstream + 22U));
    header.NPLANES       = *((uint16_t*) (imstream + 26U));
    header.NBITSPERPIXEL = *((uint16_t*) (imstream + 28U));
    header.CMPTYPE       = get_bmp_compression_kind(*((uint32_t*) (imstream + 30U)));
    header.IMAGESIZE     = *((uint32_t*) (imstream + 34U));
    header.RESPPMX       = *((uint32_t*) (imstream + 38U));
    header.RESPPMY       = *((uint32_t*) (imstream + 42U));
    header.NCMAPENTRIES  = *((uint32_t*) (imstream + 46U));
    header.NIMPCOLORS    = *((uint32_t*) (imstream + 50U));
    return header;
}

// not 100% accurate.
static __forceinline bool __stdcall is_compressed(const _In_ __BITMAPINFOHEADER bmpinfh) { return bmpinfh.IMAGESIZE ? true : false; }

typedef enum { TOPDOWN, BOTTOMUP } BMPPIXDATAORDERING;

static __forceinline BMPPIXDATAORDERING __stdcall get_pixel_order(const _In_ __BITMAPINFOHEADER bmpinfh) {
    if (bmpinfh.HEIGHT >= 0) return BOTTOMUP;
    return TOPDOWN;
}

void static inline print_bmp_info(const _In_ __BITMAPFILEHEADER bmpfh, const _In_ __BITMAPINFOHEADER bmpinfh) {
    printf_s(
        "Start marker: 424D\nFile size %Lf MiBs\nPixel data start offset: %d\n",
        ((long double) bmpfh.FSIZE) / (1024 * 1024U),
        bmpfh.PIXELDATASTART
    );
    printf_s(
        "BITMAPINFOHEADER size: %u\nImage width: %u\nImage height: %u\nNumber of planes: %hu\n"
        "Number of bits per pixel: %hu\nImage size: %u\nResolution PPM(X): %u\nResolution PPM(Y): %u\nNumber of used colormap entries: %u\n"
        "Number of important colors: %u\n",
        bmpinfh.HEADERSIZE,
        bmpinfh.WIDTH,
        bmpinfh.HEIGHT,
        bmpinfh.NPLANES,
        bmpinfh.NBITSPERPIXEL,
        bmpinfh.IMAGESIZE,
        bmpinfh.RESPPMX,
        bmpinfh.RESPPMY,
        bmpinfh.NCMAPENTRIES,
        bmpinfh.NIMPCOLORS
    );
    switch (bmpinfh.CMPTYPE) {
        case RGB       : puts("BITMAPINFOHEADER.CMPTYPE: RGB"); break;
        case RLE4      : puts("BITMAPINFOHEADER.CMPTYPE: RLE4"); break;
        case RLE8      : puts("BITMAPINFOHEADER.CMPTYPE: RLE8"); break;
        case BITFIELDS : puts("BITMAPINFOHEADER.CMPTYPE: BITFIELDS"); break;
    }
    printf_s("Number of RGBQUAD structures in colour palette: %llu\n", get_nrgbquads_in_cpalette(bmpinfh));
    printf_s("%s BMP file\n", is_compressed(bmpinfh) ? "Compressed" : "Uncompressed");
    printf_s("BMP pixel ordering: %s\n", get_pixel_order(bmpinfh) ? "BOTTOMUP" : "TOPDOWN");
    return;
}

// en enum to specify the RGB -> BW conversion method.
// AVERAGE takes the mean of R, G and B values.
// WEIGHTED_AVERAGE does GREY = (0.299 R) + (0.587 G) + (0.114 B)
// LUMINOSITY does LUM = (0.2126 R) + (0.7152 G) + (0.0722 B)
typedef enum { AVERAGE, WEIGHTED_AVERAGE, LUMINOSITY } RGBTOBWKIND;

// pixel buffer will be modified in place!
void static inline to_blacknwhite(_In_ uint8_t* restrict pixbuffer, const _In_ uint64_t size, const _In_ RGBTOBWKIND conversion_kind) {
    // ___RGBQUAD encoding assumed.
    uint8_t* buffpixregion = (pixbuffer + 54); // do not touch the headers! (the first 14 + 40 bytes)
    switch (conversion_kind) {
        case AVERAGE :
            for (uint64_t i = 0; i < (size - 54); i += 4 /* sizeof(___RGBQUAD) = 4 */) {
                buffpixregion[i] = buffpixregion[i + 1] = buffpixregion[i + 2] =
                    ((buffpixregion[i] + buffpixregion[i + 1] + buffpixregion[i + 2]) / 3); // plain arithmetic mean
            }
            break;
        case WEIGHTED_AVERAGE :
            for (uint64_t i = 0; i < (size - 54); i += 4) {
                buffpixregion[i] = buffpixregion[i + 1] = buffpixregion[i + 2] =            // weighted average
                    ((buffpixregion[i] * 0.299L) + (buffpixregion[i + 1] * 0.587L) + (buffpixregion[i + 2] * 0.114L));
            }
            break;
        case LUMINOSITY :
            for (uint64_t i = 0; i < (size - 54); i += 4) {
                buffpixregion[i] = buffpixregion[i + 1] = buffpixregion[i + 2] =
                    ((buffpixregion[i] * 0.2126L) + (buffpixregion[i + 1] * 0.7152L) + (buffpixregion[i + 2] * 0.0722L));
            }
            break;
    }
    return;
}

// the color palette in pixel buffer is in BGR order NOT RGB!

#define REMRED   (uint64_t) 0x00000010
#define REMGREEN (uint64_t) 0x00001000
#define REMBLUE  (uint64_t) 0x00100000
#define REMRG    (REMRED | REMGREEN)
#define REMRB    (REMRED | REMBLUE)
#define REMGB    (REMGREEN | REMBLUE)

// buffer will be modified in-place.
void static inline remove_color(_In_ uint8_t* restrict pixbuffer, const _In_ uint64_t size, const _In_ uint64_t rmcolor) {
    // tested, works fine :))
    int8_t* pixels_start = (pixbuffer + 54);
    switch (rmcolor) {
        case REMRED :
            for (uint64_t i = 0; i < (size - 54); i += 4) pixels_start[i + 2] = 0;
            break;
        case REMGREEN :
            for (uint64_t i = 0; i < (size - 54); i += 4) pixels_start[i + 1] = 0;
            break;
        case REMBLUE :
            for (uint64_t i = 0; i < (size - 54); i += 4) pixels_start[i] = 0;
            break;
        case REMRG :
            for (uint64_t i = 0; i < (size - 54); i += 4) pixels_start[i + 1] = pixels_start[i + 2] = 0;
            break;
        case REMRB :
            for (uint64_t i = 0; i < (size - 54); i += 4) pixels_start[i] = pixels_start[i + 2] = 0;
            break;
        case REMGB :
            for (uint64_t i = 0; i < (size - 54); i += 4) pixels_start[i] = pixels_start[i + 1] = 0;
            break;
    }
    return;
}

int wmain(_In_ int argc, _In_ wchar_t* argv[]) {
    if (argc < 2) {
        fputs("Error: no files passed", stderr);
        exit(1);
    }

    uint64_t           imsize   = 0;
    uint8_t*           bmpimage = open_image(argv[1], &imsize);

    __BITMAPFILEHEADER fhead    = parse_bitmapfile_header(bmpimage, imsize);
    __BITMAPINFOHEADER infhead  = parse_bitmapinfo_header(bmpimage, imsize);

    print_bmp_info(fhead, infhead);

    // to_blacknwhite(bmpimage, imsize, WEIGHTED_AVERAGE);

    remove_color(bmpimage, imsize, REMRG);

    wchar_t wrfname[200] = { 0 };
    wcscpy_s(wrfname, 200, argv[1]);
    uint64_t offset_null = wcsnlen_s(wrfname, 200);
    memset(wrfname + offset_null - 4, 0U, 3); // remove the .bmp extension from the filename.
    wcscat_s(wrfname, 200, L"-B.bmp");

    write_image(bmpimage, imsize, wrfname);

    free(bmpimage);
    return EXIT_SUCCESS;
}