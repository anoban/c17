#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrix4x4 {
        size_t nrows;
        size_t ncols;
        double buffer[16];
} matrix4x4_t;

static inline matrix4x4_t __stdcall matmul(_In_ const matrix4x4_t* const left, _In_ const matrix4x4_t* const right) {
    matrix4x4_t   dotprod = { .nrows = 0, .ncols = 0, .buffer = { 0 } };
    double        temp    = 0.000; // a temporary accumulator
    unsigned long caret   = 0;     // keep track of the write offset in the dot product

    if (left->ncols != right->nrows || left->nrows != right->ncols) {
        fputws(L"Error in matmul():: dimensions incompatible for matrix multiplication!\n", stderr);
        return dotprod;
    };

    dotprod.nrows = left->nrows;  // dot product will have the number of rows of the first matrix
    dotprod.ncols = right->ncols; // and the number of columns of the second matrix

    for (size_t lsoff = 0; lsoff < left->ncols * left->nrows; lsoff += left->ncols) { // left start offset
        for (size_t rsoff = 0; rsoff < count; rsoff +=) {
            for (size_t i = 0; i < count; i++) {
                for (size_t i = 0; i < count; i++) { /* code */
                }
            }
        }
    }

    dotprod.buffer[caret++] = temp;
    temp                    = 0.000;

    return dotprod;
}

static inline void print(const matrix4x4_t* const restrict matrix) {
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
    // a 4 x 4 identity matrix
    const matrix4x4_t identity = {
        .nrows = 4, .ncols = 4, .buffer = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 }
    };

    const matrix4x4_t some = {
        .nrows = 4, .ncols = 4, .buffer = { 2.0, 4.0, 6.0, 8.0 }
    };

    const matrix4x4_t dotprod = matmul(identity, some);

    print(&identity);
    _putws(L"");
    print(&some);
    _putws(L"");
    print(&dotprod);

    return EXIT_SUCCESS;
}
