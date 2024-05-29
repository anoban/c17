#define __STDC_WANT_SECURE_LIB__ 1

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define LENGTH 1000000LLU

int wmain(void) {
    LARGE_INTEGER start = { .QuadPart = 0 }, stop = { .QuadPart = 0 };
    QueryPerformanceCounter(&start);

    srand(time(NULL));
    int* const restrict randoms = malloc(LENGTH * sizeof(int));
    // we didn't handle errors in C++, so
    for (unsigned long i = 0; i < LENGTH; ++i) randoms[i] = rand();
    unsigned count = 0;
    for (unsigned long i = 0; i < LENGTH; ++i) count += (randoms[i] < (INT_MAX / 2));
    wprintf_s(L"%10u\n", count);
    free(randoms);

    QueryPerformanceCounter(&stop);
    wprintf_s(L"Execution took %lld ticks\n", stop.QuadPart - start.QuadPart);
    return EXIT_SUCCESS;
}
