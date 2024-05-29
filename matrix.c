#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct matrix {
        double*  buffer;
        unsigned nrows;
        unsigned ncols;
} matrix_t;

static_assert((bool) (sizeof(matrix_t) == 16));

// caller is responsible for freeing the new matrix
static inline matrix_t __stdcall MatMul(_In_ const matrix_t const left, _In_ const matrix_t const right) {
    // check whether the two matrices can be multiplied first,
    if (left.ncols != right.nrows || left.nrows != right.ncols) {
        fwprintf_s(
            stderr,
            L"Error in MatMul :: Shapes of the two matrices (%5u, %5u), (%5u, %5u) aren't compatible for multiplication!\n",
            left.nrows,
            left.ncols,
            right.nrows,
            right.ncols
        );
        return (matrix_t) { .buffer = NULL, .nrows = 0, .ncols = 0 };
    }

    float* const restrict result = malloc((unsigned long long) left.nrows * right.ncols * sizeof(float));
    // handle allocation errors,
    if (!result) {
        fputws(L"Error in MatMul :: memory allocation failed!", stderr);
        return (matrix_t) { .buffer = NULL, .nrows = 0, .ncols = 0 };
    }

    long double tmp = 0.0000F;

    for (unsigned r = 0; r < left.nrows; ++r) {     // NOLINT(readability-identifier-length)
        for (unsigned c = 0; c < left.nrows; ++c) { // NOLINT(readability-identifier-length)
            left.buffer[r * left.ncols + c] + right.buffer[c * right.nrows + c];

            ;
        }
    }
    return (matrix_t) { .buffer = result };
}

int wmain(void) { }
