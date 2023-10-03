#include <stdio.h>

int main(void) {
    long double cumprods[1000];

    for (int i = 1; i < 1001; ++i) cumprods[i] = 1.0L;

    for (int i = 0; i < 1000; ++i) {
        for (int j = i; j > 0; --j) {
            // Overflows quickly when using integers.
            cumprods[i] *= j;
        }
    }

    for (int i = 0; i < 1000; ++i) printf_s("%5.10e\n", cumprods[i]);

    return 0;
}