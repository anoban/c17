#define _USE_MATH_DEFINES
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>

typedef _BitInt(4) nibble; // C23 _BitInt
typedef unsigned _BitInt(4) unibble;

int wmain(void) {
    // C23 constexpr
    [[maybe_unused]] constexpr unibble ten       = 0b1010; // nibble cannot accomodate 4 bits as one is reserved to store the sign
    [[maybe_unused]] constexpr double  PIE       = M_PI;

    [[maybe_unused]] constexpr uintmax_t snibble = sizeof(nibble); // one byte though

    return EXIT_SUCCESS;
}
