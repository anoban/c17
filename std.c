#include <stdio.h>
// #include <malloc.h>
#include <math.h>

int main(void) {
    long double        mean = 0.0L, stddev = 0.0L, ssum = 0.0L;
    unsigned long long sum = 0;
    // unsigned long long* array = (unsigned long long*) malloc(1000 * sizeof(unsigned long long));

    for (int i = 1; i < 1001; ++i) {
        // array[i] = i * i;
        sum += i * i;
    }

    mean = sum / 1000;

    for (int i = 1; i < 1001; ++i) ssum += powl(((i * i) - mean), 2);

    stddev = sqrtl(ssum / 1000);

    printf_s("Mean of the array is %.10Lf\n", mean);
    printf_s("Standard deviation of the array is %.10Lf\n", stddev);

    return 0;
}
