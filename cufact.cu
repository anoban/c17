#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const size_t    SIZE    = 1000;
const size_t    MEMSIZE = SIZE * sizeof(int32_t);

__global__ void Kernel(_In_ const int32_t* const input, _Inout_ long double* const output) {
    const int32_t tindex = threadIdx.x + threadIdx.y + threadIdx.z;
    if (input[tindex] <= 0) {
        output[tindex] = 1;
        return;
    }
    long double result = 1.0L;
    for (int64_t i = 1; i <= input[tindex]; ++i) result *= i;
    output[tindex] = result;
    return;
}

int main(void) {
    srand(time(NULL));

    int32_t* const     randoms     = (int32_t*) malloc(MEMSIZE);
    long double* const results     = (long double*) malloc(SIZE * sizeof(long double));
    int32_t*           dev_rands   = NULL;
    long double*       dev_results = NULL;

    if (!randoms || !results) {
        fputws(L"malloc failed!", stderr);
        goto FREE;
    }

    if (cudaSuccess != cudaMalloc((void**) &dev_rands, MEMSIZE)) {
        fwprintf_s(stderr, L"cudaMalloc failed @ __LINE__ %3d\n", __LINE__ - 1);
        goto ERROR;
    }
    if (cudaSuccess != cudaMalloc((void**) &dev_results, SIZE * sizeof(long double))) {
        fwprintf_s(stderr, L"cudaMalloc failed @ __LINE__ %3d\n", __LINE__ - 1);
        goto ERROR;
    }

    for (size_t i = 0; i < SIZE; ++i) randoms[i] = rand() % 20;

    if (cudaSuccess != cudaMemcpy(dev_rands, randoms, MEMSIZE, cudaMemcpyHostToDevice)) {
        fwprintf_s(stderr, L"cudaMemcpy failed @ __LINE__ %3d\n", __LINE__ - 1);
        goto ERROR;
    }

    Kernel<<<1, SIZE>>>(dev_rands, dev_results);
    cudaDeviceSynchronize();

    if (cudaGetLastError() != cudaSuccess) {
        fputws(L"Kernel execution failed!", stderr);
        goto ERROR;
    }

    if (cudaSuccess != cudaMemcpy(results, dev_results, SIZE * sizeof(long double), cudaMemcpyDeviceToHost)) {
        fwprintf_s(stderr, L"cudaMemcpy failed @ __LINE__ %3d\n", __LINE__ - 1);
        goto ERROR;
    }

    for (size_t i = 0; i < SIZE; ++i) wprintf_s(L"Factorial of %4d is %12.5Lf\n", randoms[i], results[i]);
    cudaFree(dev_rands);
    cudaFree(dev_results);
    free(randoms);
    free(results);

    return EXIT_SUCCESS;

ERROR:
    cudaFree(dev_rands);
    cudaFree(dev_results);

FREE:
    free(randoms);
    free(results);
    return EXIT_FAILURE;
}