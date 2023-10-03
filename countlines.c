#include <stdio.h>
#include <Windows.h>

int wmain(void) {
    HANDLE        handle = CreateFileW(L"./helloworld.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

    LARGE_INTEGER file_size;
    GetFileSizeEx(handle, &file_size);

    size_t             buffsize = file_size.QuadPart + (1024U * 1024);
    unsigned char      buffer[buffsize]; // VLA, needs icx or clang for compilation
    unsigned long long nbytes = 0, nlines = 0;
    ReadFile(handle, buffer, buffsize, &nbytes, NULL);
    CloseHandle(handle);

    for (size_t i = 0; i < nbytes; i++) nlines += (buffer[i] == '\n');
    printf_s("Ther were %zu lines\n", nlines);
    return EXIT_SUCCESS;
}