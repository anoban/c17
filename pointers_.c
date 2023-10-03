#include <assert.h>
#include <stdio.h>
#include <Windows.h>

void static inline __stdcall means(_In_ int** arrays, _In_ int narrays, _In_ int* arr_sizes, _Out_ float* means) {
    for (int i = 0; i < narrays; ++i) {
        for (int s = 0; s < *(arr_sizes + i); ++s) *(means + i) += *(*(arrays + i) + s);
        *(means + i) /= *(arr_sizes + i);
    }
}

int main(void) {
    const long double eps     = 0.0000000001L;

    int               arr_0[] = { 0, 1, 2, 3 };
    int               arr_1[] = { 1, 3, 5, 7, 9 };
    int               arr_2[] = { 2, 4, 6, 8, 10, 12 };
    int               arr_3[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };

    int*              arrs[]  = { arr_0, arr_1, arr_2, arr_3 };

    int               sizes[] = { 4, 5, 6, 10 };
    float             mu[4]   = { 0 };

    means(arrs, 4, sizes, mu);

    printf_s("%3.15f, %3.15f, %3.15f, %3.15f\n", mu[0], mu[1], mu[2], mu[3]);

    assert((*(mu + 0) < (1.5L + eps)) && (*(mu + 0) > (1.5L - eps)));
    assert((*(mu + 1) < (5.0L + eps)) && (*(mu + 1) > (5.0L - eps)));
    assert((*(mu + 2) < (7.0L + eps)) && (*(mu + 2) > (7.0L - eps)));
    assert((*(mu + 3) < (55.0L + eps)) && (*(mu + 3) > (55.0L - eps)));

    puts("1.5, 5.0, 7.0, 55.0");

    int dummy[4][4] = { 0 };

    return 0;
}
