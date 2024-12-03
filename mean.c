#include <stdio.h>
#include <stdlib.h>
#define ARRAYSIZE 100000U

int main(void) {
    int         tmp_1  = 0;
    int         tmp_2  = 0;
    long double mean_1 = 0, mean_2 = 0, diff = 0;

    for (int i = 0; i < ARRAYSIZE; ++i) {
        mean_1 += tmp_1  = rand();
        mean_2 += tmp_2  = rand();
        diff            += tmp_1 - tmp_2;
    }

    mean_1 /= ARRAYSIZE;
    mean_2 /= ARRAYSIZE;

    printf_s("Mean of differences = %5.10Lf\n", diff / ARRAYSIZE);
    printf_s("Difference between means = %5.10Lf\n", mean_1 - mean_2);

    return 0;
}
