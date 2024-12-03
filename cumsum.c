#include <stdio.h>

int main(void) {
    unsigned long long cumsum[1000] = { 0 };

    for (int i = 0; i < 1000; ++i)
        for (int j = i; j >= 0; --j) cumsum[i] += j;

    for (int i = 0; i < 1000; ++i) printf_s("%10llu\n", cumsum[i]);

    return 0;
}
