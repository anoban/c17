#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SAMPLING_REPEATS 1000U
#define SAMPLE_SIZE      1000U
#define POPULATION_SIZE  1000000U

int main(void) {
    srand(time(NULL));

    unsigned long long randoms[POPULATION_SIZE], sum = 0;
    long double        mean = 0.0L;

    for (int i = 0; i < POPULATION_SIZE; ++i) {
        randoms[i]  = rand() % POPULATION_SIZE;
        // printf_s("%llu, ", randoms[i]);
        sum        += randoms[i];
    }

    mean = sum / POPULATION_SIZE;

    // printf_s("Sum of the population is %llu\n", sum);
    printf_s("Mean of the population is %.10Lf\n", mean);

    unsigned long long sample_sums[SAMPLING_REPEATS] = { 0 }, sample_sum = 0;
    long double        sample_means[SAMPLING_REPEATS] = { 0.0L }, sum_of_sample_means = 0.0L, mean_of_sample_means = 0.0L;

    // sample size n = 1,000.
    // population size N = 1000,000
    // 100 repeated sampling attempts.
    for (int x = 0; x < SAMPLING_REPEATS; ++x) {
        for (int i = 0; i < SAMPLE_SIZE; ++i) sample_sum += randoms[rand() % POPULATION_SIZE];

        sample_sums[x]       = sample_sum;
        sum_of_sample_means += sample_means[x] = (long double) (sample_sum / SAMPLE_SIZE);
        sample_sum                             = 0;
    }

    mean_of_sample_means = sum_of_sample_means / SAMPLING_REPEATS;

    // printf_s("Sum of the sample is %llu\n", sample_sum);
    printf_s("Mean of the means of %u samples is %.10Lf\n", SAMPLE_SIZE, mean_of_sample_means);

    return 0;
}
