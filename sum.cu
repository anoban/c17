#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 400LLU

__global__ void Kernel(_In_ const uint64_t* const input_0, _In_ const uint64_t* const input_1, _Inout_ uint64_t* const output) {
    const int thread_index = threadIdx.x + threadIdx.y + threadIdx.z;
    output[thread_index]   = input_0[thread_index] + input_1[thread_index];
    return;
}

int main(void) {
    srand(time(NULL));
    const size_t    MEMSIZE = SIZE * sizeof(uint64_t);
    uint64_t *      dev_0 = NULL, *dev_1 = NULL, *dev_results = NULL; // these pointers need to be modifiable, not constant pointers
    // these are meant to be modified my cudaMalloc()!!

    uint64_t* const randoms_0 = (uint64_t*) malloc(MEMSIZE);
    uint64_t* const randoms_1 = (uint64_t*) malloc(MEMSIZE);
    uint64_t* const device_   = (uint64_t*) malloc(MEMSIZE);

    if (!randoms_0 || !randoms_1 || !device_) {
        fputws(L"malloc returned NULL!", stderr);
        goto FREE;
    }

    for (size_t i = 0; i < SIZE; ++i) {
        randoms_0[i] = rand();
        randoms_1[i] = rand();
    }

    if (cudaSuccess != cudaMalloc((void**) &dev_0, MEMSIZE)) {
        fwprintf_s(stderr, L"cudaMalloc failed @ __LINE__ %3d\n", __LINE__ - 1);
        goto ERROR;
    }
    if (cudaSuccess != cudaMalloc((void**) &dev_1, MEMSIZE)) {
        fwprintf_s(stderr, L"cudaMalloc failed @ __LINE__ %3d\n", __LINE__ - 1);
        goto ERROR;
    }
    if (cudaSuccess != cudaMalloc((void**) &dev_results, MEMSIZE)) {
        fwprintf_s(stderr, L"cudaMalloc failed @ __LINE__ %3d\n", __LINE__ - 1);
        goto ERROR;
    }

    if (cudaSuccess != cudaMemcpy(dev_0, randoms_0, MEMSIZE, cudaMemcpyHostToDevice)) {
        fwprintf_s(stderr, L"cudaMemcpy failed @ __LINE__ %3d\n", __LINE__ - 1);
        goto ERROR;
    }
    if (cudaSuccess != cudaMemcpy(dev_1, randoms_1, MEMSIZE, cudaMemcpyHostToDevice)) {
        fwprintf_s(stderr, L"cudaMemcpy failed @ __LINE__ %3d\n", __LINE__ - 1);
        goto ERROR;
    }

    Kernel<<<1, SIZE>>>(dev_0, dev_1, dev_results);
    cudaDeviceSynchronize();

    if (cudaGetLastError() != cudaSuccess) {
        fputws(L"Kernel execution failed!", stderr);
        goto ERROR;
    }
    if (cudaSuccess != cudaMemcpy(device_, dev_results, MEMSIZE, cudaMemcpyDeviceToHost)) {
        fwprintf_s(stderr, L"cudaMemcpy failed @ __LINE__ %3d\n", __LINE__ - 1);
        goto ERROR;
    }

    for (size_t i = 0; i < SIZE; ++i) wprintf_s(L"Host sum: %8lld, Device sum: %8lld\n", randoms_0[i] + randoms_1[i], device_[i]);
    cudaFree(dev_0);
    cudaFree(dev_1);
    cudaFree(dev_results);
    free(randoms_0);
    free(randoms_1);
    free(device_);

    return EXIT_SUCCESS;

ERROR:
    cudaFree(dev_0);
    cudaFree(dev_1);
    cudaFree(dev_results);

FREE:
    free(randoms_0);
    free(randoms_1);
    free(device_);

    return EXIT_FAILURE;
}