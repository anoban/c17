#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static __forceinline size_t __stdcall calculate_block_width(_In_ const size_t image_width) {
    // we need a number less than but as close as possible to 140.
    const long double block_w = image_width / 140.0L;
    const long double nblocks = image_width / block_w;

    return floorl(image_width / floorl(nblocks));
}

int main(void) {
    srand(time(NULL));
    int64_t random = 0;
    size_t  bwidth = 0;

    for (size_t i = 0; i < 100; ++i) {
        random = rand() % 10000;
        bwidth = calculate_block_width(random);
        wprintf_s(L"Width: %5lld pixels, Block size: %5zd, remaining pixels: %5zd\n", random, bwidth, random - bwidth * 140);
    }
    return EXIT_SUCCESS;
}
