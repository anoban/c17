#include <mkl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma comment(lib, "mkl_rt")
#define NUM_ELEMENTS 100000

int main(void) {
    srand(time(NULL));
    float* const randoms = malloc(NUM_ELEMENTS * sizeof(float));
    int64_t      sum     = 0;
    for (size_t i = 0; i < NUM_ELEMENTS; ++i) sum += randoms[i] = rand();
    const uint64_t offset_min = cblas_isamin_64(NUM_ELEMENTS, randoms, 1);
    const uint64_t offset_max = cblas_isamax_64(NUM_ELEMENTS, randoms, 1);

    float          max = randoms[0], min = randoms[0];
    for (size_t i = 1; i < NUM_ELEMENTS; ++i) {
        if (randoms[i] < min) min = randoms[i];
        if (randoms[i] > max) max = randoms[i];
    }

    wprintf_s(L"min: %f, max: %f\n", min, max);
    wprintf_s(L"cblas_isamin_64: %f, cblas_isamax_64: %f\n", randoms[offset_min], randoms[offset_max]);

    free(randoms);
    return EXIT_SUCCESS;
}