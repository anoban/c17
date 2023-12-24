#pragma warning(disable : 4710)

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

static inline char* open(_In_ const wchar_t* const file_name, _Out_ uint64_t* const nread_bytes) {
    *nread_bytes    = 0;
    void *   handle = NULL, *buffer = NULL;
    uint32_t nbytes = 0;

    handle          = CreateFileW(file_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

    if (handle != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER file_size;
        if (!GetFileSizeEx(handle, &file_size)) {
            fwprintf_s(stderr, L"Error %lu in GetFileSizeEx\n", GetLastError());
            return NULL;
        }

        // add an extra megabyte to the buffer, for safety.
        size_t buffsize = file_size.QuadPart + (1024U * 1024);

        // caller is responsible for freeing this buffer.
        buffer          = malloc(buffsize);
        if (buffer) {
            if (ReadFile(handle, buffer, buffsize, (LPDWORD) &nbytes, NULL)) {
                *nread_bytes = nbytes;
                return (char*) buffer;
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

__global__ void GetCharCountKernel(_In_ const char* const buffer, _In_ const size_t* length, _Inout_ size_t* const freqs) {
    const char thread_id = threadIdx.x;
    assert(thread_id < 256);
    size_t count = 0;
    for (size_t i = 0; i < *length; ++i) count += (buffer[i] == thread_id);
    freqs[thread_id] = count;
    printf("charcter: %c, count %zd\n", thread_id, count);
    return;
}

int main(void) {
    size_t            frequencies[256] = { 0 };

    size_t            nbytes           = 0;
    const char* const buffer           = open(L"./moby_dick.txt", &nbytes);
    if (!buffer) exit(100);

    char*   device_buffer = NULL;
    size_t* device_freqs  = NULL;
    size_t* device_nbytes = 0;
    cudaMalloc(&device_nbytes, sizeof(size_t));
    cudaMemcpy(device_buffer, buffer, nbytes, cudaMemcpyHostToDevice);
    cudaMalloc(&device_freqs, 256 * sizeof(size_t));
    cudaMemcpy(device_nbytes, &nbytes, sizeof nbytes, cudaMemcpyHostToDevice);

    GetCharCountKernel<<<2, 256>>>(device_buffer, device_nbytes, device_freqs);

    cudaDeviceSynchronize();
    cudaMemcpy(frequencies, device_freqs, 256 * sizeof(size_t), cudaMemcpyDeviceToHost);
    cudaFree(device_freqs);
    cudaFree(device_buffer);
    cudaFree(device_nbytes);
    cudaDeviceReset();

    for (size_t i = 0; i < 256; ++i) wprintf_s(L"Frequency of character %c is %10zu.\n", i, frequencies[i]);

    for (size_t i = 0; i < nbytes; ++i) frequencies[buffer[i]]++;
    free((void*) buffer);

    for (size_t i = 0; i < 256; ++i) wprintf_s(L"Frequency of character %C is %10zu.\n", i, frequencies[i]);
    return EXIT_SUCCESS;
}