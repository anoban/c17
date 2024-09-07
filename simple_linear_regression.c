#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static const long double SLOPE         = 5.76415178263481;
static const long double INTERCEPT     = -0.76415178263481;
static const long double LEARNING_RATE = 0.15000;

#define MAX_SIZE       20'000
#define MAX_ITERATIONS 1'000
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

static __forceinline long double __stdcall loss(
    _In_ const long double const x, _In_ const long double const y, _In_ register const long double a, _In_ register const long double b
) {
    return powl(y - (a * x + b), 2.00L) / 2.0000L;
}

static inline long double __stdcall cost(
    _In_count_(length) const long double* const x,
    _In_count_(length) const long double* const y,
    _In_ register const size_t                  length,
    _In_ register const long double             a,
    _In_ register const long double             b
) {
    // loss = (prediction - target) ^ 2
    // cost = ((prediction - target) ^ 2) / 2 i.e cost is averaged aggregate loss
    long double cost = 0.0000;
    for (size_t i = 0; i < length; ++i) cost += loss(x[i], y[i], a, b); // loss aggregation
    return cost / length;                                               // averaging
}

// but the derivative of the cost function with respect to Y_hat is -(Y - Y_hat)
// which makes all the squaring unnecessary!

typedef struct pair {
        long double value_0;
        long double value_1;
} pair_t;

static inline pair_t __stdcall deriv_cost(
    _In_count_(length) const long double* const x,
    _In_count_(length) const long double* const y,
    _In_ register const size_t                  length,
    _In_ register const long double             a,
    _In_ register const long double             b
) {
    // derivative of the cost function with respect to the weight and bias (i.e. our prediction)
    long double dcost_w = 0.0000, dcost_b = 0.0000, dcost = 0.000;

    for (size_t i = 0; i < length; ++i) {
        dcost    = y[i] - (x[i] * a + b); // -(Y_i - Y_hat_i)
        dcost_b -= dcost;                 // -(Y_i - Y_hat_i)
        dcost_w -= (dcost * x[i]);        // -(Y_i - Y_hat_i) * x_i
    }

    return (pair_t) { dcost_w / length, dcost_b / length }; // averaging
}

int wmain(void) {
    srand(time(NULL));

    const long double* const predictor = generate_x(MAX_SIZE);
    const long double* const target    = generate_y(predictor, MAX_SIZE, SLOPE, INTERCEPT);

    // boostrap a and b with two random values between 0 and 1.0
    long double learned_a = rand() / RAND_MAXF, learned_b = rand() / RAND_MAXF;
    wprintf_s(L"bootstrapped a = %2.10Lf, b = %2.10Lf\n", learned_a, learned_b);
    pair_t d_error = { 0.000 };

    // gradient descent
    for (size_t i = 0; i < MAX_ITERATIONS; ++i) {
        d_error    = deriv_cost(predictor, target, MAX_SIZE, learned_a, learned_b);
        // calculate the derivatives and update learned_a and learned_b
        learned_a -= d_error.value_0 * LEARNING_RATE;
        learned_b -= d_error.value_1 * LEARNING_RATE;
    }

    free(predictor);
    free(target);

    wprintf_s(L"actual       a = %2.10Lf, b = %2.10Lf\n", SLOPE, INTERCEPT);
    wprintf_s(L"inferred     a = %2.10Lf, b = %2.10Lf\n", learned_a, learned_b);

    return EXIT_SUCCESS;
}
