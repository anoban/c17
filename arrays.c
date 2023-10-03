#include <stdint.h>
#include <stdio.h>

long double intSignedArraySum(char* array, uint64_t size, uint8_t stride) {
    long double sum = 0.0;

    switch (stride) {
        case 8 :
            int64_t* i64array = (int64_t*) array;
            for (uint64_t i = 0; i < size; ++i) {
                sum += *i64array;
                i64array++;
            }
            break;
        case 4 :
            int32_t* i32array = (int32_t*) array;
            for (uint64_t i = 0; i < size; ++i) {
                sum += *i32array;
                i32array++;
            }
            break;
        case 2 :
            int16_t* i16array = (int16_t*) array;
            for (uint64_t i = 0; i < size; ++i) {
                sum += *i16array;
                i16array++;
            }
            break;
        case 1 :
            int8_t* i8array = (int8_t*) array;
            for (uint64_t i = 0; i < size; ++i) {
                sum += *i8array;
                i8array++;
            }
            break;
    }

    return sum;
}

int main(void) {
    int32_t     array[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };

    long double sum     = 0.0;
    for (uint8_t stride = 1; stride < 9; stride *= 2) {
        sum = intSignedArraySum((char*) array, 10, stride);
        printf_s("Sum %10.5Lf when stride is %u\n", sum, stride);
    }

    return 0;
}