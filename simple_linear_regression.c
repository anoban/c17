#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const long double SLOPE         = 5.76415178263481;
static const long double INTERCEPT     = -0.76415178263481;
static const long double LEARNING_RATE = 0.0125;

#define MAX_SIZE       20'000
#define MAX_ITERATIONS 10'000
#define RAND_MAXF      32767.0L // (long double) RAND_MAX

static inline long double* __cdecl generate_x(_In_ register const size_t length) {
    long double* array = malloc(length * sizeof(long double));
    if (!array) {
        //
        return NULL;
    }
    for (size_t i = 0; i < length; ++i) array[i] = rand() / RAND_MAXF;
    return array;
}

static inline long double* __cdecl generate_y(
    _In_count_(length) register const long double* const x,
    _In_ register const size_t                           length,
    _In_ register const long double                      a,
    _In_ register const long double                      b
) {
    long double* array = malloc(length * sizeof(long double));
    if (!array) {
        //
        return NULL;
    }
    for (size_t i = 0; i < length; ++i) array[i] = x[i] * a + b;
    return array;
}

static inline long double __stdcall loss(
    _In_count_(length) const long double* const x,
    _In_count_(length) const long double* const y,
    _In_ register const size_t                  length,
    _In_ register const long double             a,
    _In_ register const long double             b
) {
    // ((prediction - target) ^ 2) / 2 * length
    long double cost = 0.0000;
    for (size_t i = 0; i < length; ++i) cost += powl(y[i] - (x[i] * a + b), 2.00L);
    return cost / (2.000L * length);
}

int wmain(void) {
    const long double* const predictor = generate_x(MAX_SIZE);
    const long double* const target    = generate_y(predictor, MAX_SIZE, SLOPE, INTERCEPT);

    // boostrap a and b with two random values between 0 and 1.0
    long double leanred_a = rand() / RAND_MAXF, learned_b = rand() / RAND_MAXF;
    long double error = 0.000;

    // gradient descent
    for (size_t i = 0; i < MAX_ITERATIONS; ++i) {
        //
        error = loss(predictor, target, MAX_SIZE, leanred_a, learned_b);
        // calculate the derivatives and update learned_a and learned_b
    }

    free(predictor);
    free(target);

    return EXIT_SUCCESS;
}
