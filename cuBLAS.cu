//Example 1. Application Using C and cuBLAS: 1-based indexing
//-----------------------------------------------------------
#include <cuda_runtime.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "cublas_v2.h"
#define M               6
#define N               5
#define IDX2F(i, j, ld) ((((j) - 1) * (ld)) + ((i) - 1))

static __inline__ void modify(cublasHandle_t handle, float* m, int ldm, int n, int p, int q, float alpha, float beta) {
    cublasSscal_v2(handle, n - q + 1, &alpha, &m[IDX2F(p, q, ldm)], ldm);
    cublasSscal_v2(handle, ldm - p + 1, &beta, &m[IDX2F(p, q, ldm)], 1);
}

int main(void) {
    cudaError_t    cudaStat;
    cublasStatus_t stat;
    cublasHandle_t handle;
    int            i, j;
    float*         devPtrA;
    float*         a = 0;

    a                = (float*) malloc(M * N * sizeof(*a));
    if (!a) {
        printf_s("host memory allocation failed");
        return EXIT_FAILURE;
    }

    for (j = 1; j <= N; j++)
        for (i = 1; i <= M; i++) a[IDX2F(i, j, M)] = (float) ((i - 1) * N + j);

    cudaStat = cudaMalloc((void**) &devPtrA, M * N * sizeof(*a));
    if (cudaStat != cudaSuccess) {
        printf_s("device memory allocation failed");
        return EXIT_FAILURE;
    }

    stat = cublasCreate_v2(&handle);
    if (stat != CUBLAS_STATUS_SUCCESS) {
        printf_s("CUBLAS initialization failed\n");
        return EXIT_FAILURE;
    }

    stat = cublasSetMatrix(M, N, sizeof(*a), a, M, devPtrA, M);
    if (stat != CUBLAS_STATUS_SUCCESS) {
        printf_s("data download failed");
        cudaFree(devPtrA);
        cublasDestroy_v2(handle);
        return EXIT_FAILURE;
    }
    modify(handle, devPtrA, M, N, 2, 3, 16.0f, 12.0f);

    stat = cublasGetMatrix(M, N, sizeof(*a), devPtrA, M, a, M);
    if (stat != CUBLAS_STATUS_SUCCESS) {
        printf_s("data upload failed");
        cudaFree(devPtrA);
        cublasDestroy_v2(handle);
        return EXIT_FAILURE;
    }

    cudaFree(devPtrA);
    cublasDestroy_v2(handle);

    for (j = 1; j <= N; j++) {
        for (i = 1; i <= M; i++) printf_s("%7.0f", a[IDX2F(i, j, M)]);
        printf_s("\n");
    }

    free(a);
    return EXIT_SUCCESS;
}
