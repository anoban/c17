#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int wmain(void) {
    long double sum = 0.000;

    for (size_t i = 0; i < UCHAR_MAX; i++) {
        for (size_t j = 0; j < UCHAR_MAX; j++) {
            for (size_t k = 0; k < UCHAR_MAX; k++) {
                //
                for (size_t l = 0; l < UCHAR_MAX; l++) {
                    for (size_t m = 0; m < UCHAR_MAX; m++)
                        for (size_t n = 0; n < UCHAR_MAX; n++) sum += 1.00;
                }
            }
        }
    }

    wprintf_s(L"sum is %.5Lf\n", sum);
    return EXIT_SUCCESS;
}
