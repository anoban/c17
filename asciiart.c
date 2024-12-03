// a simple application that prints the ASCII art of a Windows .BMP file.

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define WIN32_EXTRA_MEAN
    #include <Windows.h>
#endif

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline uint8_t* OpenImage(_In_ const wchar_t* const restrict file_name, _Out_ uint64_t* const nread_bytes) {
    *nread_bytes = 0;
    void *handle = NULL, *buffer = NULL;
    DWORD nbytes = 0;

    handle       = CreateFileW(file_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

    if (handle != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER file_size;
        if (!GetFileSizeEx(handle, &file_size)) {
            fwprintf_s(stderr, L"Error %lu in GetFileSizeEx\n", GetLastError());
            return NULL;
        }

        // caller is responsible for freeing this buffer.
        buffer = malloc(file_size.QuadPart);
        if (buffer) {
            if (ReadFile(handle, buffer, file_size.QuadPart, &nbytes, NULL)) {
                *nread_bytes = nbytes;
                return buffer;
            } else {
                fwprintf_s(stderr, L"Error %lu in ReadFile\n", GetLastError());
                CloseHandle(handle);
                free(buffer);
                return NULL;
            }
        } else {
            fputws(L"Memory allocation error: malloc returned NULL", stderr);
            CloseHandle(handle);
            return NULL;
        }
    } else {
        fwprintf_s(stderr, L"Error %lu in CreateFileW\n", GetLastError());
        return NULL;
    }
}

static inline BITMAPFILEHEADER ParseBitmapFileHeader(_In_ const uint8_t* restrict imstream, _In_ const uint64_t fsize) {
    static_assert(sizeof(BITMAPFILEHEADER) == 14LLU, "Error: BITMAPFILEHEADER is not 14 bytes in size.");
    assert(fsize >= sizeof(BITMAPFILEHEADER));

    BITMAPFILEHEADER header = { .bfType = 0, .bfSize = 0, .bfReserved1 = 0, .bfReserved2 = 0, .bfOffBits = 0 };

    header.bfType           = (((uint16_t) (*(imstream + 1))) << 8) | ((uint16_t) (*imstream));
    if (header.bfType != (((uint16_t) 'M' << 8) | (uint16_t) 'B')) {
        fputws(L"Error in parse_bitmapfile_header, file appears not to be a Windows BMP file\n", stderr);
        return header;
    }
    header.bfSize    = *((uint32_t*) (imstream + 2));
    header.bfOffBits = *((uint32_t*) (imstream + 10));
    return header;
}

static inline BITMAPINFOHEADER ParseBitmapInfoHeader(_In_ const uint8_t* const restrict imstream, _In_ const uint64_t fsize) {
    static_assert(sizeof(BITMAPINFOHEADER) == 40LLU, "Error: __BITMAPINFOHEADER is not 40 bytes in size");
    assert(fsize >= (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)));

    BITMAPINFOHEADER header = {
        .biSize          = 0,
        .biWidth         = 0,
        .biHeight        = 0,
        .biPlanes        = 0,
        .biBitCount      = 0,
        .biCompression   = 0,
        .biSizeImage     = 0,
        .biXPelsPerMeter = 0,
        .biYPelsPerMeter = 0,
        .biClrUsed       = 0,
        .biClrImportant  = 0,
    };
    if (*((uint32_t*) (imstream + 14U)) > 40) {
        fputws(L"BMP image seems to contain an unparsable file info header", stderr);
        return header;
    }
    header.biSize          = *((uint32_t*) (imstream + 14U));
    header.biWidth         = *((uint32_t*) (imstream + 18U));
    header.biHeight        = *((int32_t*) (imstream + 22U));
    header.biPlanes        = *((uint16_t*) (imstream + 26U));
    header.biBitCount      = *((uint16_t*) (imstream + 28U));
    header.biCompression   = *((uint32_t*) (imstream + 30U));
    header.biSizeImage     = *((uint32_t*) (imstream + 34U));
    header.biXPelsPerMeter = *((uint32_t*) (imstream + 38U));
    header.biYPelsPerMeter = *((uint32_t*) (imstream + 42U));
    header.biClrUsed       = *((uint32_t*) (imstream + 46U));
    header.biClrImportant  = *((uint32_t*) (imstream + 50U));

    return header;
}

// A struct representing a BMP image.
typedef struct _BMP {
        size_t           fsize;
        size_t           npixels;
        BITMAPFILEHEADER fhead;
        BITMAPINFOHEADER infhead;
        RGBQUAD*         pixel_buffer;
} WinBMP;

static inline WinBMP NewBmpImage(
    _In_ const uint8_t* const restrict imstream /* will be freed by this procedure */, _In_ const size_t size
) {
    assert(imstream);
    const BITMAPFILEHEADER fh    = ParseBitmapFileHeader(imstream, size);
    const BITMAPINFOHEADER infh  = ParseBitmapInfoHeader(imstream, size);
    WinBMP                 image = { .fsize = size, .npixels = (size - 54) / 4, .fhead = fh, .infhead = infh };
    image.pixel_buffer           = malloc(size - 54);
    if (image.pixel_buffer) {
        memcpy_s(image.pixel_buffer, size - 54, imstream + 54, size - 54);
        ;
    } else {
        fwprintf_s(stderr, L"Error in %s @ line %d: malloc falied!\n", __FUNCTIONW__, __LINE__);
        return (WinBMP) {
            0,
            0,
            { 0, 0, 0, 0, 0 },
            {
              0, 0,
              0, 0,
              0, 0,
              0, 0,
              0, 0,
              0, },
            NULL
        };
    }
    return image;
}

static inline void BmpInfo(_In_ const WinBMP* const restrict image) {
    wprintf_s(
        L"File size %Lf MiBs\nPixel data start offset: %d\n"
        L"BITMAPINFOHEADER size: %u\nImage width: %u\nImage height: %u\nNumber of planes: %hu\n"
        L"Number of bits per pixel: %hu\nImage size: %u\nResolution PPM(X): %u\nResolution PPM(Y): %u\nNumber of used colormap entries: % u\n"
        L"Number of important colors: % u\n",
        (long double) (image->fhead.bfSize) / (1024 * 1024LLU),
        image->fhead.bfOffBits,
        image->infhead.biSize,
        image->infhead.biWidth,
        image->infhead.biHeight,
        image->infhead.biPlanes,
        image->infhead.biBitCount,
        image->infhead.biSizeImage,
        image->infhead.biXPelsPerMeter,
        image->infhead.biYPelsPerMeter,
        image->infhead.biClrUsed,
        image->infhead.biClrImportant
    );

    switch (image->infhead.biCompression) {
        case 0  : _putws(L"BITMAPINFOHEADER.CMPTYPE: RGB"); break;
        case 1  : _putws(L"BITMAPINFOHEADER.CMPTYPE: RLE4"); break;
        case 2  : _putws(L"BITMAPINFOHEADER.CMPTYPE: RLE8"); break;
        case 3  : _putws(L"BITMAPINFOHEADER.CMPTYPE: BITFIELDS"); break;
        default : _putws(L"BITMAPINFOHEADER.CMPTYPE: UNKNOWN"); break;
    }

    wprintf_s(
        L"%s BMP file\n"
        L"BMP pixel ordering: %s\n",
        image->infhead.biSizeImage != 0 ? L"Compressed" : L"Uncompressed",
        image->infhead.biHeight >= 0 ? L"BOTTOMUP" : L"TOPDOWN"
    );

    return;
}

// ASCII characters in descending order of luminance
// static const char ascii[] = { 'N', '@', '#', 'W', '$', '9', '8', '7', '6', '5', '4', '3', '2', '1',
//                               '?', '!', 'a', 'b', 'c', ';', ':', '+', '=', '-', ',', '.', '_' };

static const char ascii[] = { '_', '.', ',', '-', '=', '+', ':', ';', 'c', 'b', 'a', '!', '?', '1',
                              '2', '3', '4', '5', '6', '7', '8', '9', '$', 'W', '#', '@', 'N' };

static __forceinline char __stdcall ScaleRgbQuad(_In_ const RGBQUAD* const restrict pixel) {
    return ascii[((((long) pixel->rgbBlue) + pixel->rgbGreen + pixel->rgbRed) / 3) % 27];
}

typedef struct _ascii_t {
        const char*  buffer;
        const size_t length; // count of wchar_t s in the buffer.
} ascii_t;

static inline ascii_t GenerateASCIIBuffer(_In_ const WinBMP* const restrict image) {
    // TODO
    // we need to downscale the ascii art if the image is larger than the console window
    // a fullscreen cmd window is 215 chars wide and 50 chars high
    char* txtbuff = malloc((image->infhead.biHeight * image->infhead.biWidth + image->infhead.biHeight) * sizeof(char));
    if (!txtbuff) {
        fwprintf_s(stderr, L"Error in %s @ line %d: malloc failed!\n", __FUNCTIONW__, __LINE__);
        return (ascii_t) { NULL, 0 };
    }

    size_t h = 0, w = 0;
    for (; h < image->infhead.biHeight; ++h) {
        w = 0;
        for (; w < image->infhead.biWidth; ++w)
            txtbuff[h * image->infhead.biWidth + w] = ScaleRgbQuad(&image->pixel_buffer[h * image->infhead.biWidth + w]);
        // txtbuff[h * w + 1] = '\n';
    }
    return (ascii_t) { txtbuff, (image->infhead.biHeight * image->infhead.biWidth + image->infhead.biHeight) };
}

int wmain(_In_opt_ const int32_t argc, _In_opt_count_(argc) wchar_t* argv[]) {
    uint64_t fsize = 0;

    for (size_t i = 1; i < argc; ++i) {
        const uint8_t* buffer = OpenImage(argv[i], &fsize);

        if (buffer) {
            const WinBMP image = NewBmpImage(buffer, fsize);

            if (image.pixel_buffer) {
                BmpInfo(&image);

                const ascii_t txt = GenerateASCIIBuffer(&image);
                if (txt.buffer) {
                    for (size_t i = 0; i < txt.length; ++i) {
                        // putwchar(txt.buffer[i]);
                        // printf_s("%d: %c ", txt.buffer[i], txt.buffer[i]);
                        putchar(txt.buffer[i]);
                    }
                    free(txt.buffer);
                }
                free(image.pixel_buffer);
            }

        } else {
            fwprintf_s(stderr, L"Skipping image %s\n", argv[i]);
            continue;
        }
    }

    return EXIT_SUCCESS;
}
