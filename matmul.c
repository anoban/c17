#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrix {
        size_t nrows;
        size_t ncols;
        double buffer[16];
} matrix_t;

static inline matrix_t __stdcall matmul(_In_ const matrix_t left, _In_ const matrix_t right) {
    matrix_t      dotprod = { .nrows = 0, .ncols = 0, .buffer = { 0 } };
    double        temp    = 0.000; // a temporary accumulator
    unsigned long caret   = 0;     // keep track of the write offset in the dot product

    if (left.ncols != right.nrows) {
        fputws(L"Error in matmul():: dimensions incompatible for matrix multiplication!\n", stderr);
        return dotprod;
    };

    dotprod.nrows          = left.nrows;  // dot product will have the number of rows of the first matrix
    dotprod.ncols          = right.ncols; // and the number of columns of the second matrix

    const unsigned leftec  = left.nrows * left.ncols;
    const unsigned rightec = right.nrows * right.ncols;

    for (unsigned leftr = 0; leftr < leftec; leftr += left.ncols) { // for each row in the first matrix,
        for (unsigned reps = 0; reps < right.ncols; ++reps) {       // for each column in the second matrix

            for (unsigned r = leftr; r < leftr + left.ncols; ++r) {
                for (unsigned c = reps; c < rightec; c += right.ncols) {
                    temp += left.buffer[r] * right.buffer[c];
                    //
                }
            }
            dotprod.buffer[caret++] = temp;
            temp                    = 0.000;
        }
    }

    return dotprod;
}

static inline void print(const matrix_t* const restrict matrix) {
    static unsigned long caret          = 0;
    static wchar_t       rowbuffer[150] = { 0 };

    for (unsigned i = 0; i < matrix->nrows; ++i) {
        rowbuffer[caret++] = L'[';
        rowbuffer[caret++] = L' ';
        for (unsigned j = 0; j < matrix->ncols; ++j) {
            //
            caret += swprintf_s(rowbuffer + caret, __crt_countof(rowbuffer), L"%04.2lf, ", matrix->buffer[i * matrix->ncols + j]);
        }
        rowbuffer[caret]   = L' ';
        rowbuffer[caret++] = L']';

        _putws(rowbuffer);
        memset(rowbuffer, 0, __crt_countof(rowbuffer) * sizeof(wchar_t));
        caret = 0;
    }
}

int wmain(void) {
    const matrix_t identity = {
        // a 4 x 4 identity matrix
        .nrows  = 2,
        .ncols  = 2,
        .buffer = { 1.0, 0.0, 0.0, 1.0 }
    };

    const matrix_t some = {
        .nrows = 2, .ncols = 2, .buffer = { 2.0, 4.0, 6.0, 8.0 }
    };

    const matrix_t dotprod = matmul(identity, some);

    print(&identity);
    _putws(L"");
    print(&some);
    _putws(L"");
    print(&dotprod);

    return EXIT_SUCCESS;
}
