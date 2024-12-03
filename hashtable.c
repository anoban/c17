#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define WIN32_EXTRA_MEAN
    #include <Windows.h>
#endif
#define BUFFER_SIZE 2048000U

/*
unsigned long __forceinline hash(char* word) {

}
*/

int main(void) {
    HANDLE hFile = CreateFileA("D:/C/Moby Dick.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) fprintf_s(stderr, "Error %lu in accessing file.\n", GetLastError());

    char* buffer = (char*) malloc(BUFFER_SIZE);
    memset(buffer, 0, BUFFER_SIZE);

    DWORD dwBytesRead = 0;
    ReadFile(hFile, buffer, BUFFER_SIZE, &dwBytesRead, NULL);
    CloseHandle(hFile);
    printf_s("%lu bytes read from file!.\n", dwBytesRead);
    // puts(buffer);

    char CharactersToNullify[] = { '.', ',', '?', '`', '"', '\'', '\n', '\r', '(', ')', '[', ']', ' ', '-', '_', '{', '}', '*' };

    for (size_t i = 0; i < dwBytesRead; ++i) {
        for (size_t j = 0; j < 17; ++j) {
            if (buffer[i] == CharactersToNullify[j]) {
                buffer[i] = 0;
                break;
            }
        }
    }

    puts(buffer);

    free(buffer);
    return 0;
}
