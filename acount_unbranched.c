#include <stdbool.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#define BUFF_SIZE 2000000U

int main(void) {
    HANDLE file = CreateFileA("D:/C/Moby Dick.txt", GENERIC_READ, 0U, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file == INVALID_HANDLE_VALUE) {
        fprintf_s(stderr, "Error %lu in accessing the specified file.\n", GetLastError());
        ExitProcess(1);
    }

    char          buffer[BUFF_SIZE] = { 0 };
    unsigned long nBytesRead        = 0;
    if (!ReadFile(file, buffer, BUFF_SIZE, &nBytesRead, NULL)) {
        fprintf_s(stderr, "Error %lu in reading file contents.\n", GetLastError());
        ExitProcess(1);
    }

    printf_s("%lu bytes read from the file.\n", nBytesRead);

    if (!CloseHandle(file)) fprintf_s(stderr, "Error %lu in closing the file handle.\n", GetLastError());

    LARGE_INTEGER t_start, t_end, frequency, elapsed;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&t_start);

    char          characters[128];
    unsigned long count[128] = { 0 };

    for (char i = 0; i < 127; ++i) characters[i] = i;

    bool iseq = false;

    for (unsigned long i = 0; i < nBytesRead; ++i) {
        for (short j = 31; j < 128; ++j) {
            iseq      = (buffer[i] == characters[j]);
            count[j] += iseq;
            j        += iseq * 300;

            /*
            if (buffer[i] == characters[j]) {
                count[j] += 1;
                break;
            }
            */
        }
    }

    QueryPerformanceCounter(&t_end);
    elapsed.QuadPart  = t_end.QuadPart - t_start.QuadPart;
    elapsed.QuadPart *= 1000000U;

    printf_s("Unbranched version took %Lf micro seconds.\n", ((long double) elapsed.QuadPart) / frequency.QuadPart / 1000);

    for (short i = 31; i < 128; ++i)
        printf_s("There are %6lu %cs (%d) in Herman Melville's Moby Dick.\n", count[i], characters[i], characters[i]);

    return 0;
}
