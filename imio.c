#include <image.h>

// io helpers

void* open_image(_In_ const wchar_t* file_name, _Out_ uint64_t* nread_bytes) {
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
#ifdef __VERBOSE_STD_OUTPUTS
                wprintf_s(L"Loaded %ls (%.3Lf MiBs)\n", file_name, ((long double) nbytes) / (1024 * 1024));
#endif // __VERBOSE_STD_OUTPUTS

                *nread_bytes = nbytes;
                CloseHandle(handle);
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

bool write_image(_In_ const uint8_t* restrict buffer, _In_ const uint64_t buffsize, _In_ const wchar_t* restrict file_name) {
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

    CloseHandle(hfile);
    return true;
}
