#pragma once
#include <master.h>
#ifndef __JPEG__
    #define __JPEG__

// a header containing function prototypes and data structure definitions for jpeg.c

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
        uint8_t  UNITS; // units for the x and y pixel densities.
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

// frequency table to store counts for each byte

bool           is_jpeg(_In_ const uint8_t* restrict jpeg_stream, _In_ const uint64_t nbytes);
JFIF_APP0_HEAD parse_jfif_app0_header(_In_ const uint8_t* restrict jpeg_stream, _In_ const uint64_t nbytes);
void           print_jpeg_info(_In_ const JFIF_APP0_HEAD app0_head);
int64_t        find_jfif_sof_pos(_In_ const uint8_t* restrict jpeg_stream, _In_ const uint64_t size);

#endif // !__JPEG__
