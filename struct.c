// clang .\struct.c -Wall -std=c23 -Wextra -Wpedantic -O3

#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#define ARRAY_LENGTH CHAR_MAX // 127

typedef struct container {
        double  dvalue;
        int64_t ivalue;
        float   array[ARRAY_LENGTH];
} container_t;

#ifdef __TRYME__

static inline container_t func(void) {
    float dummy[ARRAY_LENGTH] = { 0 };
    for (size_t i = 0; i < ARRAY_LENGTH; ++i) dummy[i] = (float) i; // array will contain 0.0 to 127.0
    return (container_t) { .dvalue = DBL_MAX, .ivalue = INT64_MAX, .array = { dummy } };
    //  error: initializing 'float' with an expression of incompatible type 'float[127]'
}

#endif // !__TRYME__

static inline container_t funcv2(void) {
    container_t tmp = { .dvalue = DBL_MAX, .ivalue = INT64_MAX };
    for (size_t i = 0; i < ARRAY_LENGTH; ++i) tmp.array[i] = (float) i;
    return tmp;
}

int main(void) {
    const container_t result = funcv2();
    for (size_t i = 0; i < ARRAY_LENGTH; ++i) wprintf_s(L"%4.4f\n", result.array[i]);
    return 0;
}
