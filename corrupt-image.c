#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

void* open_image(const _In_ wchar_t* file_name, _Out_ uint64_t* nread_bytes) {
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
        size_t buffsize = file_size.QuadPart + (1024Ui64 * 1024);

        // caller is responsible for freeing this buffer.
        buffer          = malloc(buffsize);
        if (buffer) {
            if (ReadFile(handle, buffer, buffsize, &nbytes, NULL)) {
                *nread_bytes = nbytes;
                return buffer;
            }

            else {
                fprintf_s(stderr, "Error %lu in ReadFile\n", GetLastError());
                CloseHandle(handle);
                free(buffer);
                return NULL;
            }
        }

        else {
            fputs("Memory allocation error: malloc returned NULL", stderr);
            CloseHandle(handle);
            return NULL;
        }
    }

    else {
        fprintf_s(stderr, "Error %lu in CreateFileW\n", GetLastError());
        return NULL;
    }
}

int main(void) {
    size_t imsize = 0, wfsize = 0;
    char*  image = open_image(L"./sydney.jpg", &imsize);

    // let's corrupt 100 bytes and see what happens!
    for (size_t i = 0; i < 100; ++i) image[rand() % imsize] = rand() % 0xFF;

    HANDLE wfile = CreateFileW(L"./corrupted.jpeg", GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

    WriteFile(wfile, image, imsize, &wfsize, NULL);

    CloseHandle(wfile);
    free(image);

    return EXIT_SUCCESS;
}