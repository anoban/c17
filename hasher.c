#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_MEAN
#include <Windows.h>
#define BUFFER_SIZE 10485760U // 10 MiBs
#define SIZE_HASHES 380000U

unsigned long long __forceinline FNV1aHash(const char* string, unsigned long long length) {
    const unsigned long long FNV_OFFSET_BASIS = 0XCBF29CE484222325;
    const unsigned long long FNV_PRIME        = 0X100000001B3;
    unsigned long long       hash             = FNV_OFFSET_BASIS;

    for (unsigned long long i = 0; i < length; ++i) {
        hash ^= string[i];
        hash *= FNV_PRIME;
    }

    return hash;
}

bool __forceinline ReadTextFile(const wchar_t* file_name, char** stream, unsigned long* bytes_read) {
    // Caller needs to free the stream buffer, once usage is finished.
    *stream = (char*) malloc(BUFFER_SIZE);
    if (!*stream) {
        fprintf_s(stderr, "Memory allocation error: %lu\n", GetLastError());
        return false;
    }

    memset(*stream, 0, BUFFER_SIZE);

    HANDLE hFile = CreateFileW(file_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf_s(stderr, "Error %lu in accessing file!\n", GetLastError());
        free(*stream);
        return false;
    }

    if (!ReadFile(hFile, *stream, BUFFER_SIZE, bytes_read, NULL)) {
        fprintf_s(stderr, "Error %lu in reading file.\n", GetLastError());
        free(*stream);
        return false;
    }

#ifdef _DEBUG
    printf_s("%lu bytes has been read from the file.\n", *bytes_read);
    puts(*stream);
#endif
    return true;
}

int main(void) {
    char* buffer             = NULL;
    // printf_s("%s\n", buffer);

    unsigned long read_bytes = 0;
    if (!ReadTextFile(L"D:/C/true-words.txt", &buffer, &read_bytes)) return 1;

    // printf_s("%s\n", buffer);

    unsigned long long* hashes = (unsigned long long*) malloc(SIZE_HASHES * 8);
    memset(hashes, 0U, SIZE_HASHES);
    unsigned long long last_hash_offset         = 0;

    unsigned long long current_word_stop_offset = 0, next_word_start_offset = 0;

    for (unsigned long long i = 0; i < read_bytes; ++i) {
        if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
            current_word_stop_offset = i - 1;

            hashes[last_hash_offset] =
                FNV1aHash((buffer + next_word_start_offset), current_word_stop_offset - next_word_start_offset) % SIZE_HASHES;

            next_word_start_offset = (i + 2);

            last_hash_offset++;
            i++; // skip the '\n'

        } else {
            continue;
        }
    }

    free(buffer);

    for (unsigned long i = 0; i < SIZE_HASHES; ++i) {
        if (!hashes[i]) {
            /* printf_s("Hash at offset %lu is %llu\n", i, hashes[i]);
        } else {
            */
            printf_s("Offset %lu is empty.\n", i);
        }
    }

    free(hashes);
    return 0;
}
