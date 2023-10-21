#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_MEAN
#include <Windows.h>

// caller needs to free the pointer returned, before using the buffer, check for NULL.
static inline void* open(const _In_ wchar_t* restrict file_name, _Inout_ uint32_t* restrict nbytes) {
    void* file_handle = CreateFileW(file_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

    if (file_handle == INVALID_HANDLE_VALUE) {
        fwprintf_s(stderr, L"Error %lu in CreateFileW\n", GetLastError());
        goto cleanup;
    }

    LARGE_INTEGER file_size;
    if (!GetFileSizeEx(file_handle, &file_size)) {
        fwprintf_s(stderr, L"Error %lu in GetFileSizeEx\n", GetLastError());
        goto cleanup;
    }

    void* buffer = malloc(file_size.QuadPart + 1048576LLU);
    if (!buffer) {
        fputws(L"Malloc returned NULL", stderr);
        goto cleanup;
    }

    if (!ReadFile(file_handle, buffer, file_size.QuadPart, nbytes, NULL)) {
        fwprintf_s(stderr, L"Error %lu in ReadFile\n", GetLastError());
        free(buffer);
        goto cleanup;
    }

    CloseHandle(file_handle);
    return buffer;

cleanup:
    CloseHandle(file_handle);
    return NULL;
}

static inline uint64_t __stdcall BE2LE_U64(const _In_ uint64_t be_value) {
    // e.g. BE: 1111 0000 1010 1001 1010 1000 0101 0101 1111 1011 1101 1111 0001 1001 0111 0011
    //			----1---- ----2---- ----3---- ----4---- ----5---- ----6---- ----7---- ----8----
    uint64_t le_value  = (be_value << 56U);                           // make the last BE byte first.
    le_value          |= ((be_value << 40U) & 0x00FF000000000000LLU); // make the 7th BE byte second.
    le_value          |= ((be_value << 24U) & 0x0000FF0000000000LLU); // make the 6th BE byte third.
    le_value          |= ((be_value << 8U) & 0x000000FF00000000LLU);  // make the 5th BE byte fourth.
    le_value          |= ((be_value >> 8U) & 0x00000000FF000000LLU);  // make the 4th BE byte fifth.
    le_value          |= ((be_value >> 24U) & 0x0000000000FF0000LLU); // make the 3rd BE byte sixth.
    le_value          |= ((be_value >> 40U) & 0x000000000000FF00LLU); // make the 2nd BE byte seventh.
    le_value          |= (be_value >> 56U);                           // make the first byte last.
    return le_value;
}

// void* buffers will implicitly be converted to uint64_t*, and the size must be in bytes.
static inline void hex_dump(const _In_ uint64_t* buffer, const _In_ uint64_t buffsize) {
    // Hex of a 64 bit integer will take 16 characters. A fullscreen terminal window is roughly 140 characters wide.
    // We can print 6 columns of 64 bit hex values, neatly.

    // if there are n bytes, each byte will take 2 wide characters to represent.
    const uint64_t wstrbuffsize           = sizeof(wchar_t) * buffsize * 4; // Excess, of course. Done deliberately.
    const uint64_t wstrbuffsize_in_wchars = wstrbuffsize / 2;

    wchar_t*       wstring_buffer         = malloc(wstrbuffsize);
    if (!wstring_buffer) { };                                               // hanlde it.

    memset(wstring_buffer, 0U, wstrbuffsize);
    uint64_t caret = 0;
    for (uint64_t i = 0; i < (buffsize / 8); i += 6) {
        // swprintf_s returns the number of wide characters stored in buffer.
        caret += swprintf_s(
            wstring_buffer + caret,
            wstrbuffsize_in_wchars - caret,
            L"%10llu. %016llX  %016llX  %016llX  %016llX  %016llX  %016llX\n",
            i * 8,
            buffer[i],
            buffer[i + 1],
            buffer[i + 2],
            buffer[i + 3],
            buffer[i + 4],
            buffer[i + 5]
        );
    }
    _putws(wstring_buffer);
    free(wstring_buffer);
    return;
}

// when the programme is invoked with -Help
static inline void help(void) {
    _putws(
        L"hexdump.exe -Help"
        L"-File   FilePath [String]:              Full/relative path of the input file\n"
        L"-First  N [Positive integer]:           First N lines will be printed\n"
        L"-Last   N [Positive integer]:           Last N lines will be printed\n"
        L"-Text   True[Default]/False [Boolean]:  Flag to show the ASCII representation of file buffer on right\n"
        L"-Search [HX/AS]:String [String:String]: Search for a given pattern in the buffer\n"
        L"                                        HX/AS specifies whether the argument is to be interpreted as a Hex value or a ASCII sequence of characters\n"
        L"Example: hexdump.exe -File dump.bin -Search HX:FF1AD819 will look for these 4 contiguous bytes in the file buffer\n"
        L"Example: hexdump.exe -File dump.bin -Search AS:IDAT will look for the ASCII character sequence \"IDAT\" in the file buffer\n"
        L"-Search tokens MUST NOT be enclosed in quotes\n"
        L"-Search does accept multiple tokens simultaneously. To combine a series of tokens use a list syntax: [,]\n"
        L"Example: hexdump.exe -File dump.bin -Search HX:[AA12DA98,5674D30AB3C8]"
        L"Example: hexdump.exe -File dump.bin -Search AS:[IDAT,IHDR]"
        L"Example: hexdump.exe -File dump.bin -Search AS:[IDAT,gAMA] HX:[AA12DA98,74D3AEB3C8F7]"
        L"Any errors in argument parsing will lead to premature programme termination\n"
    );
    return;
}

// Accepts just one file at a time.
int wmain(int32_t argc, wchar_t* argv[]) {
    switch (argc) {
        case 1  : fputws(L"Error: hexdump.exe No files passed.", stderr); return EXIT_FAILURE;
        case 2  : break;
        default : fputws(L"Error: hexdump.exe does not accept more than one file at once!.", stderr); return EXIT_FAILURE;
    }

    uint32_t fsize = 0;
    void*    file  = NULL;

    if ((file = open(argv[1], &fsize))) {
        hex_dump(file, fsize);
    } else {
        fwprintf_s(stderr, L"Error %lu in reading file %ls\n", GetLastError(), argv[1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
