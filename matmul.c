#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct matrix {
        size_t nrows;
        size_t ncols;
        float* buffer;
} matrix_t;

static inline matrix_t __stdcall matmul(_In_ const matrix_t left, _In_ const matrix_t right) {
    matrix_t temp = { .nrows = 0, .ncols = 0, .buffer = NULL };
    if (left.ncols != right.nrows) {
        fputws(
            L"Error in matmul():: for a matrix multiplication to be valid, the number of columns in the first matrix and the number of rows in the second matrix must be equal",
            stderr
        );
        return temp;
    };
    // the matrix multiplication product will have the number of rows of the first matrix and the number of columns of the second matrix
    float* const buffer = malloc(left.nrows * right.ncols * sizeof(float));
    if (!buffer) {
        fputws(L"Error in matmul():: memory allocation error :: malloc returned NULL", stderr);
        return temp;
    }

    temp.nrows  = left.nrows;
    temp.ncols  = right.ncols;
    temp.buffer = buffer;

    return temp;
}

int main(void) { return EXIT_SUCCESS; }
