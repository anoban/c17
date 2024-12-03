#include <image.h>
#include <png.h>
#include <utils.h>

bool validate_png_signature(_In_ const uint8_t* restrict png_stream, _In_ const uint64_t fsize) {
    if (fsize < 10) return false;
    return (png_stream[0] == 137) && (png_stream[1] == 80) && (png_stream[2] == 78) && (png_stream[3] == 71) && (png_stream[4] == 13) &&
           (png_stream[5] == 10) && (png_stream[6] == 26) && (png_stream[7] == 10);
}

PNG_CHUNK_HEAD extract_IHDR(_In_ const uint8_t* restrict png_stream, _In_ const uint64_t fsize) {
    PNG_CHUNK_HEAD ihdr_head = { .LENGTH = 0, .NAME = { 0 }, .DATA = NULL, .CRC32 = 0 };
    if (fsize < 100) return ihdr_head;
    const uint8_t* ihdr_start = png_stream + 8; // first 8 bytes were for the PNG signature.

    // FUCK THIS BIG ENDIAN SHIT, PNG uses BIG ENDIAN byte order.
    ihdr_head.LENGTH          = BE2LE_U32(*((uint32_t*) ihdr_start));

    memcpy(ihdr_head.NAME, ihdr_start + 4, 4U);
    ihdr_head.DATA  = ihdr_start + 8;
    ihdr_head.CRC32 = BE2LE_U32(*((uint32_t*) (ihdr_head.DATA + ihdr_head.LENGTH)));
    return ihdr_head;
}

IHDR_DATA extract_IHDR_data(_In_ const PNG_CHUNK_HEAD ihdr_head) {
    // TODO: handle empty structs.
    IHDR_DATA ihdr_data = {
        .WIDTH = 0, .HEIGHT = 0, .BITDEPTH = 0, .COLORTYPE = 0, .COMPRESSION_METHOD = 0, .FILTER_METHOD = 0, .INTERLACE_METHOD = 0
    };
    if (ihdr_head.LENGTH != 13) {
        fputws(L"Error: IHDR DATA section must be 13 bytes wide.", stderr);
        return ihdr_data;
    }
    const uint8_t* data_start = ihdr_head.DATA;
    if ((*(data_start + 10)) || (*(data_start + 12))) {
        // compression method & filter method need to be 0.
        fputws(L"Error: Compression method & filter method need to be 0 for PNG files.\n", stderr);
        return ihdr_data;
    }
    ihdr_data.WIDTH              = BE2LE_U32(*((uint32_t*) data_start));
    ihdr_data.HEIGHT             = BE2LE_U32(*((uint32_t*) (data_start + 4)));
    ihdr_data.BITDEPTH           = *(data_start + 8);
    ihdr_data.COLORTYPE          = *(data_start + 9);
    ihdr_data.COMPRESSION_METHOD = *(data_start + 10);
    ihdr_data.FILTER_METHOD      = *(data_start + 11);
    ihdr_data.INTERLACE_METHOD   = *(data_start + 12);
    return ihdr_data;
}

void print_IHDR_info(_In_ const PNG_CHUNK_HEAD ihdr_head, _In_ const IHDR_DATA ihdr_data) {
    wprintf_s(
        L"HEADER NAME: %S\nDATA LENGTH : %10u\nCRC32 : %10u\n"
        L"IMAGE WIDTH: %10u\nIMAGE HEIGHT: %10u\nBIT DEPTH: %d\nINTERLACING: %s\n"
        L"COMPRESSION METHOD: %d\nFILTER METHOD: %d\n",
        ihdr_head.NAME,
        ihdr_head.LENGTH,
        ihdr_head.CRC32,
        ihdr_data.WIDTH,
        ihdr_data.HEIGHT,
        ihdr_data.BITDEPTH,
        ihdr_data.INTERLACE_METHOD ? L"ADAM7" : L"NONE",
        ihdr_data.COMPRESSION_METHOD,
        ihdr_data.FILTER_METHOD
    );
    switch (ihdr_data.COLORTYPE) {
        case IHDRCT_GREYSCALE       : _putws(L"COLORTYPE: GREYSCALE"); break;
        case IHDRCT_RGBTRIPLE       : _putws(L"COLORTYPE: RGBTRIPLE"); break;
        case IHDRCT_PALETTE         : _putws(L"COLORTYPE: PALETTE"); break;
        case IHDRCT_GREYSCALE_ALPHA : _putws(L"COLORTYPE: GREYSCALE_ALPHA"); break;
        case IHDRCT_RGB_ALPHA       : _putws(L"COLORTYPE: RGB_ALPHA"); break;
        default                     : _putws(L"COLORTYPE: UNKNOWN");
    }
    return;
}
