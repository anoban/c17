// clang .\rvalues.c -O3 -Wall -Wextra -pedantic -std=c23 -o .\rvalues.exe

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// without struct packing, using raw offsets caluclated using sizeof() on members won't help much when paddings are involved
#pragma pack(push, 1)
typedef struct packed {
        unsigned long x;
        long double   y;
        char          z[10];
} packed_t;
#pragma pack(pop)

typedef struct regular {
        unsigned long x; // 4 bytes padding added after data member x
        long double   y;
        char          z[10]; // 6 bytes padding added after data member z
} regular_t;

static inline packed_t __stdcall GetPacked(void) { return (packed_t) { .x = 12345, .y = 6.3764527645L, .z = { "Anoban" } }; }

static inline regular_t __stdcall GetRegular(void) { return (regular_t) { .x = 12345, .y = 6.3764527645L, .z = { "Anoban" } }; }

int main(void) {
    // const packed_t* lvalue = &(GetPacked());	// Error: cannot take the address of an rvalue of type 'packed_t' (aka 'struct packed')
    const packed_t      some  = GetPacked();
    const unsigned long memex = GetPacked().x;
    printf_s("%lu\n", memex);

    const char* const zee = GetPacked().z; // string literals are lvalues!
    puts(zee);
    const char* const zed = GetRegular().z;
    puts(zed);

    // if the return values of GetPacked() were stored in memory, the unused member variables could be addressed too!!
    const long double* const maybe_y = (long double*) (zee - sizeof(long double));
    printf_s("%.10Lf 6.3764527645\n", *maybe_y); // access violations huh?
    const unsigned long* const maybe_x = (unsigned long*) (zee - sizeof(long double) - sizeof(unsigned long));
    printf_s("%lu 12345\n", *maybe_x); // we are winning son!
    // fucking works woohooo!!!!!

    const char* const name = GetRegular().z;
    puts(name); // will work fine :)
    // now is the tricky part
    const long double* const willbe_y =
        (long double*) (name - sizeof(long double)); // will work because no padding bytes were added after y
    const unsigned long* const willbe_x = (unsigned long*) (name - sizeof(long double) - sizeof(unsigned long) - 4
    ); // will work because the 4 padding bytes added after x are accounted for
    printf_s("%.10Lf 6.3764527645\n", *willbe_y);
    printf_s("%lu 12345\n", *willbe_x);

    // const double*       ptr  = &3.14765847; // cannot do this in C or C++
    // const double* const _ptr = &3.14765847;

    return EXIT_SUCCESS;
}
