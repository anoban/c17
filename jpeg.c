#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

#pragma comment(lib, "User32")
#pragma warning(disable : 4820 4710)

// JFIF markers are conventionally of two bytes.
// the first byte is almost always 0xFF
// and the next byte is indicative of the marker type.

// JFIF markers can be classified into two types.
// 1) stand-alone markers: these markers are not followed by a cognate data segment.
// 2) non standa alone markers: these are two byte markers with a downstream data segment,
// these markers are followed by another two bytes encoding the size of the data segment.
// this value includes the two bytes encoding the data size, but not the marker's two bytes.

// a JFIF format can have up to 16 APP markers, APP0 to APP15
// APP0 marker is critical as it's an essential component of the JFIF encoding.
// remaining APP markers, if present, serve application specific purposes.
// these can be safely ignored, if deemed unnecessary.

// JFIF APPn markers delimit the start of a APPn header.
// APPn markers share the format 0xFF

// a 16 byte long JFIF APP0 header.
typedef struct {
        char     TYPE[5]; // "JFIF\0"
        uint8_t  VERSION_MAJOR;
        uint8_t  VERSION_MINOR;
        uint8_t  UNITS;   // units for the x and y pixel densities.
        // 0 means no densities are used.
        uint16_t XDENSITY;       // horizontal pixel density
        uint16_t YDENSITY;       // vertical pixel density
        uint8_t  OPTTHUMBNAIL_X; // width of the optional thumbnail, could be 0.
        uint8_t  OPTTHUMBNAIL_Y; // height of the optional thumbnail, could be 0.
        uint8_t* THUMBNAIL;      // a variable sized buffer containing the thumbnail
                                 // sizeof(THUMBNAIL) is 3 * OPTTHUMBNAIL_X * OPTTHUMBNAIL_Y
                                 // buffer is made of a series RGB pixels, hence the "3 *" part of the buffer size.
                                 // needs a malloc ed memory block.
} JFIF_APP0_HEAD;

// to be used in APP0 extension header.
typedef enum { JPEG = 0x10, BYTE_PER_PIXEL, BYTES3_PER_PIXEL } JFIF_APP0EXT_THUMBNAIL_ENC;
// 0x10 - thumbnail is encodes as JPEG.
// 0x11 - thumbnail is encoded with one byte per pixel.
// 0x12 - thumbnail is encoded with three bytes per pixel.

// JFIF APP0 extension header. (optional)
typedef struct {
        char                       TYPE[5]; // "JFXX\0"
        JFIF_APP0EXT_THUMBNAIL_ENC ENCODE;  // specifies the format of the EXT_DATA
        uint8_t*                   EXT_DATA;
} JFIF_APP0EXT_HEAD;

// the data referred as EXT_DATA in the JFIF_APP0EXT_HEAD can be defined as structs

// for BYTE_PER_PIXEL encoded thumbnails,
typedef struct {
        uint8_t THUMBNAIL_X;
        uint8_t THUMBNAIL_Y;
        uint8_t PALETTE[768]; // array of RGB triplet bytes.
        uint8_t NPIXELS;      // THUMBNAIL_X * THUMBNAIL_Y
                              // type of NPIXELS is variable.
} EXT_DATATYPE_0x11;

// for BYTES3_PER_PIXEL encoded thumbnails,
typedef struct {
        uint8_t  THUMBNAIL_X;
        uint8_t  THUMBNAIL_Y;
        uint8_t* PIXELS; // an array of RGB color values for each pixel in the image
                         // size = 3 * THUMBNAIL_X * THUMBNAIL_Y
} EXT_DATATYPE_0x12;

static __forceinline void imerror_show(_In_ wchar_t* restrict failed_proc) {
    wchar_t buffer[150]       = { 0 };
    wchar_t final_buffer[350] = { 0 };
    // https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessage
    FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
        buffer,
        150U,
        NULL
    );

    swprintf_s(final_buffer, 350U, L"Error %lu occured in call to %ls\n%ls\n", GetLastError(), failed_proc, buffer);
    // hWnd can be NULL if the message box has no other windows.
    // lpCaption can

    int resp = MessageBoxExW(NULL, final_buffer, failed_proc, MB_ICONERROR | MB_OK, MAKELANGID(SUBLANG_DEFAULT, SUBLANG_SYS_DEFAULT));
    if (resp == IDOK) return;
}

static __forceinline void* open_image(const _In_ wchar_t* file_name, _Out_ uint64_t* nread_bytes) {
    *nread_bytes    = 0;
    void *   handle = NULL, *buffer = NULL;
    uint32_t nbytes = 0;

    handle          = CreateFileW(file_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

    if (handle != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER file_size;
        if (!GetFileSizeEx(handle, &file_size)) {
            imerror_show(L"GetFileSizeEx");
            // fprintf_s(stderr, "Error %lu in GetFileSizeEx\n", GetLastError());
            return NULL;
        }

        // add an extra megabyte to the buffer, for safety.
        size_t buffsize = file_size.QuadPart + (1024LLU * 1024);

        // caller is responsible for freeing this buffer.
        buffer          = malloc(buffsize);
        if (buffer) {
            if (ReadFile(handle, buffer, buffsize, &nbytes, NULL)) {
                *nread_bytes = nbytes;
                return buffer;
            } else {
                imerror_show(L"ReadFile");
                // fprintf_s(stderr, "Error %lu in ReadFile\n", GetLastError());
                CloseHandle(handle);
                free(buffer);
                return NULL;
            }
        } else {
            imerror_show(L"malloc");
            // fputs("Memory allocation error: malloc returned NULL", stderr);
            CloseHandle(handle);
            return NULL;
        }
    }

    else {
        imerror_show(L"CreateFileW");
        // fprintf_s(stderr, "Error %lu in CreateFileW\n", GetLastError());
        return NULL;
    }
}

static __forceinline bool write_image(
    _Inout_ uint8_t* restrict buffer, const _In_ uint64_t buffsize, const _In_ wchar_t* restrict file_name
) {
    HANDLE hfile = CreateFileW(file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hfile == INVALID_HANDLE_VALUE) {
        imerror_show(L"CreateFileW");
        // fprintf_s(stderr, "Error %lu in CreateFileW\n", GetLastError());
        return false;
    }

    uint32_t nbyteswritten = 0;
    if (!WriteFile(hfile, buffer, buffsize, &nbyteswritten, NULL)) {
        imerror_show(L"WriteFile");
        // fprintf_s(stderr, "Error %lu in WriteFile\n", GetLastError());
        CloseHandle(hfile);
        return false;
    }
    return true;
}

static __forceinline bool is_jpeg(const _In_ uint8_t* restrict jpeg_stream, const _In_ uint64_t nbytes) {
    const uint16_t SOJPEG = 0xFFD8, EOJPEG = 0xFFD9;
    uint16_t       first16bits = 0, last16bits = 0;

    first16bits = (((uint16_t) jpeg_stream[0]) << 8) | ((uint16_t) jpeg_stream[1]);
    last16bits  = (((uint16_t) jpeg_stream[nbytes - 2]) << 8) | ((uint16_t) jpeg_stream[nbytes - 1]);

    if ((first16bits == SOJPEG) && (last16bits == EOJPEG)) return true;
    return false;
}

static __forceinline JFIF_APP0_HEAD parse_jfif_app0_header(const _In_ uint8_t* restrict jpeg_stream, const _In_ uint64_t nbytes) {
    JFIF_APP0_HEAD jfif_app0 = { .TYPE           = { 0 },
                                 .VERSION_MAJOR  = 0,
                                 .VERSION_MINOR  = 0,
                                 .UNITS          = 0,
                                 .XDENSITY       = 0,
                                 .YDENSITY       = 0,
                                 .OPTTHUMBNAIL_X = 0,
                                 .OPTTHUMBNAIL_Y = 0,
                                 .THUMBNAIL      = NULL };

    // first two bytes of jpeg_stream will be the SOI 0xFF, 0xD8
    // these are followed by APP0 marker 0xFF, 0xE0
    // skip the next two bytes,
    // we'll have the JFIF string. (jpeg_stream + 6)
    // this string ends @ jpeg_stream + 6 + 5 with its null terminator.

    strcpy_s(jfif_app0.TYPE, 5, jpeg_stream + 6);
    jfif_app0.VERSION_MAJOR  = jpeg_stream[11];
    jfif_app0.VERSION_MINOR  = jpeg_stream[12];
    jfif_app0.UNITS          = jpeg_stream[13];
    jfif_app0.XDENSITY       = *((uint16_t*) (jpeg_stream + 14));
    jfif_app0.YDENSITY       = *((uint16_t*) (jpeg_stream + 16));
    jfif_app0.OPTTHUMBNAIL_X = jpeg_stream[18];
    jfif_app0.OPTTHUMBNAIL_Y = jpeg_stream[19];
    if (jfif_app0.OPTTHUMBNAIL_X && jfif_app0.OPTTHUMBNAIL_Y) {
        const uint64_t thumbnail_size = 3LLU * jfif_app0.OPTTHUMBNAIL_X * jfif_app0.OPTTHUMBNAIL_Y;
        jfif_app0.THUMBNAIL           = malloc(thumbnail_size);
        // TODO: handle malloc failure.
        memcpy_s(jfif_app0.THUMBNAIL, thumbnail_size, jpeg_stream + 20, thumbnail_size);
    }
    return jfif_app0;
}

static __forceinline void print_jpeg_info(const _In_ JFIF_APP0_HEAD app0_head) {
    printf_s(
        "File format: %s\n"
        "Version: %d.%d\n"
        "Pixel density unit: %d\n"
        "Pixel density x: %hd, y: %hd\n"
        "Embedded thumbnail: %s\n"
        "Thumbnail dimensions x: %d, y: %d\n"
        "Thumbnail size: %llu bytes.\n",
        app0_head.TYPE,
        app0_head.VERSION_MAJOR,
        app0_head.VERSION_MINOR,
        app0_head.UNITS,
        app0_head.XDENSITY,
        app0_head.YDENSITY,
        app0_head.THUMBNAIL ? "present" : "absent",
        app0_head.OPTTHUMBNAIL_X,
        app0_head.OPTTHUMBNAIL_Y,
        3LLU * app0_head.OPTTHUMBNAIL_X * app0_head.OPTTHUMBNAIL_Y
    );

    return;
}

// start of frame (in big endian format)
// 0xFFC0 - baseline
// 0xFFC1 - extended sequential
// 0xFFC2 - progressive
// 0xFFC3 - loseless
static __forceinline int64_t find_jfif_sof_pos(const _In_ uint8_t* restrict jpeg_stream, const _In_ uint64_t size) {
    for (uint64_t i = 0; i < size; ++i)
        if ((jpeg_stream[i] == 0xFF) && (jpeg_stream[i + 1] >= 0xC0) && (jpeg_stream[i + 1] <= 0xC2)) return i;
    return -1;
}

int wmain(int32_t argc, wchar_t* argv[]) {
    if (argc != 2) exit(1);

    uint64_t fsize = 0;
    uint8_t* image = open_image(argv[1], &fsize);

    wprintf_s(L"%ls is %ls .jpeg file\n", argv[1], is_jpeg(image, fsize) ? L"a" : L"not a");
    _putws(L"");

    JFIF_APP0_HEAD app0 = parse_jfif_app0_header(image, fsize);
    print_jpeg_info(app0);

    _putws(L"");

    // this is interesting..
    uint64_t markerpos = 0;
    wchar_t* SOF       = L"SOF";
    wchar_t* DHT       = L"DHT";

    for (uint64_t i = 0; i < fsize; ++i) {
        if (image[i] == 0xFF && image[i + 1] != 0x00) {
            wprintf_s(L"%2X%2X @ %8llu, diff: %8llu\n", image[i], image[i + 1], i, i - markerpos);
            markerpos = i;
        }
    }
    _putws(L"");

    uint64_t fospos = find_jfif_sof_pos(image, fsize);
    wprintf_s(L"SOF marker %2X%2X found @ pos: %llu", image[fospos], image[fospos + 1], fospos);
    _putws(L"");

    free(image);
    return EXIT_SUCCESS;
}