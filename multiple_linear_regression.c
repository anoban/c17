#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// build a model that can predict the fish weight (Perch) given its dimensions

#define NROWS          56LLU
#define MAX_PREDICTORS 10LLU
static const long double RAND_MAXF = 32767.0L; // (long double) RAND_MAX

static const float weights[NROWS]  = { 5.9,  32.,  40.,  51.5, 70.,  100., 78.,  80.,   85.,  85.,   110.,  115.,  125.,  130.,
                                       120., 120., 130., 135., 110., 130., 150., 145.,  150., 170.,  225.,  145.,  188.,  180.,
                                       197., 218., 300., 260., 265., 250., 250., 300.,  320., 514.,  556.,  840.,  685.,  700.,
                                       700., 690., 900., 650., 820., 850., 900., 1015., 820., 1100., 1000., 1100., 1000., 1000. };

static const float length_0[NROWS] = { 7.5,  12.5, 13.8, 15.,  15.7, 16.2, 16.8, 17.2, 17.8, 18.2, 19.,  19.,  19.,  19.3,
                                       20.,  20.,  20.,  20.,  20.,  20.5, 20.5, 20.7, 21.,  21.5, 22.,  22.,  22.6, 23.,
                                       23.5, 25.,  25.2, 25.4, 25.4, 25.4, 25.9, 26.9, 27.8, 30.5, 32.,  32.5, 34.,  34.,
                                       34.5, 34.6, 36.5, 36.5, 36.6, 36.9, 37.,  37.,  37.1, 39.,  39.8, 40.1, 40.2, 41.1 };

static const float length_1[NROWS] = { 8.4,  13.7, 15.,  16.2, 17.4, 18.,  18.7, 19.,  19.6, 20.,  21.,  21., 21.,  21.3,
                                       22.,  22.,  22.,  22.,  22.,  22.5, 22.5, 22.7, 23.,  23.5, 24.,  24., 24.6, 25.,
                                       25.6, 26.5, 27.3, 27.5, 27.5, 27.5, 28.,  28.7, 30.,  32.8, 34.5, 35., 36.5, 36.,
                                       37.,  37.,  39.,  39.,  39.,  40.,  40.,  40.,  40.,  42.,  43.,  43., 43.5, 44. };

static const float length_2[NROWS] = { 8.8,  14.7, 16.,  17.2, 18.5, 19.2, 19.4, 20.2, 20.8, 21.,  22.5, 22.5, 22.5, 22.8,
                                       23.5, 23.5, 23.5, 23.5, 23.5, 24.,  24.,  24.2, 24.5, 25.,  25.5, 25.5, 26.2, 26.5,
                                       27.,  28.,  28.7, 28.9, 28.9, 28.9, 29.4, 30.1, 31.6, 34.,  36.5, 37.3, 39.,  38.3,
                                       39.4, 39.3, 41.4, 41.4, 41.3, 42.3, 42.5, 42.4, 42.5, 44.6, 45.2, 45.5, 46.,  46.6 };

static const float height[NROWS]   = { 2.112,  3.528,   3.824,   4.5924,  4.588,   5.2224,  5.1992,  5.6358,  5.1376,  5.082,
                                       5.6925, 5.9175,  5.6925,  6.384,   6.11,    5.64,    6.11,    5.875,   5.5225,  5.856,
                                       6.792,  5.9532,  5.2185,  6.275,   7.293,   6.375,   6.7334,  6.4395,  6.561,   7.168,
                                       8.323,  7.1672,  7.0516,  7.2828,  7.8204,  7.5852,  7.6156,  10.03,   10.2565, 11.4884,
                                       10.881, 10.6091, 10.835,  10.5717, 11.1366, 11.1366, 12.4313, 11.9286, 11.73,   12.3808,
                                       11.135, 12.8002, 11.9328, 12.5125, 12.604,  12.4888 };

static const float width[NROWS]    = { 1.408,  1.9992, 2.432,  2.6316, 2.9415, 3.3216, 3.1234, 3.0502, 3.0368, 2.772,  3.555,  3.3075,
                                       3.6675, 3.534,  3.4075, 3.525,  3.525,  3.525,  3.995,  3.624,  3.624,  3.63,   3.626,  3.725,
                                       3.723,  3.825,  4.1658, 3.6835, 4.239,  4.144,  5.1373, 4.335,  4.335,  4.5662, 4.2042, 4.6354,
                                       4.7716, 6.018,  6.3875, 7.7957, 6.864,  6.7408, 6.2646, 6.3666, 7.4934, 6.003,  7.3514, 7.1064,
                                       7.225,  7.4624, 6.63,   6.8684, 7.2772, 7.4165, 8.142,  7.5958 };

typedef struct coeffs {
        long double w_len0; // length 0
        long double w_len1;
        long double w_len2;
        long double w_h; // height
        long double w_w; // width
        long double b;   // bias
} coeffs_t;

// Y_hat = w_len0 * length0 + w_len1 * length1 + w_len2 * length2 + w_h * height + w_w * width + b
// we have 5 weights and a bias as model parameters

// using variadics here is a dumb idea
static inline coeffs_t __stdcall compute_derivatives(
    _In_ const coeffs_t                   params,
    _In_count_(length) const float* const target,
    _In_ const size_t                     length,
    _In_ const size_t                     argc /* 5 */,
    ... /* the five predictor arrays */

) {
    static const float* arrays[MAX_PREDICTORS] = { NULL }; // enough space to store 10 pointers
    coeffs_t            temp                   = { 0.000 };

    va_list argv                               = NULL;
    __va_start(&argv, argc);
    for (size_t i = 0; i < argc; ++i) arrays[i] = va_arg(argv, const float*); // collect the predictors in `array`

    long double dcost = 0.000L; // -(Y - Y_hat)
    for (size_t i = 0; i < NROWS; ++i) {
        //
        dcost = ;
    }
}

int wmain(void) {
    srand((unsigned) time(NULL));

    // bootstrap the model parameters
    coeffs_t params = { .w_len0 = rand() / RAND_MAXF,
                        .w_len1 = rand() / RAND_MAXF,
                        .w_len2 = rand() / RAND_MAXF,
                        .w_h    = rand() / RAND_MAXF,
                        .w_w    = rand() / RAND_MAXF,
                        .b      = rand() / RAND_MAXF };

    return EXIT_SUCCESS;
}
