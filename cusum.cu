#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

// cannot push an MX330 above 2 gigs :(

#define T          1024LLU   // number of threads
#define N          349525LLU // numbers each thread has to sum
#define memsize(x) (N * T * sizeof(x))

__global__ static void kernel(_In_ const double* const nums, _Inout_ double* const results) {
    const unsigned index        = threadIdx.x + threadIdx.y + threadIdx.z;
    const unsigned start_offset = index * N;
    double         res          = 0.00;
    for (size_t i = start_offset; i < start_offset + N; ++i) res += nums[i];
    results[index] = res;
}

__global__ static void reduce(_In_count_(nnums) const double* const nums, _In_ const size_t nnums, _Inout_ double* const result) {
    double res = 0.00;
    for (size_t i = 0; i < nnums; ++i) res += nums[i];
    *result = res;
}

int wmain() {
    srand(time(nullptr));
    double* const numbers = (double*) malloc(memsize(double));
    if (!numbers) { }

    double*     dnumbers    = nullptr;
    double*     dthreadsums = nullptr;
    double*     dgrandsum   = nullptr;
    long double hsum = 0.000L, dsum = 0.000L;
    cudaError_t status = cudaSuccess;

    // fill the array with real numbers between range 0 and 1
    for (unsigned i = 0; i < N; ++i) hsum += numbers[i] = rand() / (double) RAND_MAX;

    status = cudaMalloc(&dnumbers, memsize(double));
    if (status != cudaSuccess) { }

    status = cudaMemcpy(dnumbers, numbers, memsize(double), cudaMemcpyHostToDevice);
    if (status != cudaSuccess) { }

    status = cudaMalloc(&dthreadsums, T * sizeof(double));
    if (status != cudaSuccess) { }

    kernel<<<1, T>>>(dnumbers, dthreadsums);
    status = cudaGetLastError();
    if (status != cudaSuccess) { }

    status = cudaDeviceSynchronize();
    if (status != cudaSuccess) { }

    status = cudaMalloc(&dgrandsum, sizeof(double));
    if (status != cudaSuccess) { }

    reduce<<<1, 1>>>(dthreadsums, T, dgrandsum);
    status = cudaGetLastError();
    if (status != cudaSuccess) { }

    status = cudaMemcpy(&dsum, dgrandsum, sizeof(double), cudaMemcpyDeviceToHost);
    if (status != cudaSuccess) { }

    cudaFree(dnumbers);
    cudaFree(dthreadsums);
    cudaFree(dgrandsum);

    wprintf_s(L"host :: %Lf, device :: %Lf\n", hsum, dsum); // :)

    return EXIT_SUCCESS;
}
