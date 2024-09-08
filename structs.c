#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

struct phone {
        short yom;
        char  brand[20];
        char  model[20];
};

typedef struct point {
        int x, y, z;
} point_t;

union fruit {
        bool banana;
        bool apple;
        bool orange;
};

typedef union fishes {
        bool salmon;
        bool mackarel;
        bool tuna;
        bool herring;
        bool skate;
} fish_t;

typedef union arrays {
        int   iarray[4];
        float farray[4];
} array_t;

typedef union xmmval {
        // A union to represent values stored in an xmm register.
        // (a 128 bits wide register)

        // One such register can hold either of these following arrays.

        int8_t  mi8[16];
        int16_t mi16[8];
        int32_t mi32[4];
        int64_t mi64[2];

        uint8_t  mu8[16];
        uint16_t mu16[8];
        uint32_t mu32[4];
        uint64_t mu64[2];

        float  mf32[4];
        double mf64[2];

} xmm_t;

int main(void) {
    struct phone M10      = { .brand = "Samsung", .model = "M10", .yom = 2021 };
    struct point centre   = { .x = 0, .y = 0, .z = 0 };
    point_t      position = { .x = 10, .y = 67, .z = -53 };

    printf_s("Phone: %s %s %d\n", M10.brand, M10.model, M10.yom);

    union fruit kathali;
    kathali.banana = true;

    union fishes dinner;
    dinner.skate      = true;

    fish_t lunch      = { .tuna = true };

    union xmmval xmm0 = {
        .mi8 = { 'a', 'n', 'o', 'b', 'a', 'n', '@', 'g', 'm', 'a', 'i', 'l', '.', 'c', 'o', 0 }
    };

    union arrays arr_0 = { .iarray = { 0 } };
    array_t      arr_1 = {
             .iarray = { 24, 76, 786, 8120 }
    };

    array_t arr_2;
    // arr_2 = { .farray = { 767.7587f, 0.65765f } };

    arr_2.farray[0] = 6897.7865f;
    arr_2.farray[1] = 0.7432f;

    array_t arr_3;
    // arr_3.farray = { 7673.65f, 675.0839f };

    puts((char*) &xmm0);

    return 0;
}