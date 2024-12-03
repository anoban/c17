#include <stdint.h>
#include <stdio.h>

int64_t __declspec(dllexport) summer(int32_t* array, int32_t size) {
    int64_t sum     = 0;
    int32_t counter = 0;
    do {
        sum += *(array + counter);
        counter++;
    } while (counter < size);
    return sum;
}

// When compiling this as a DLL, you'll need an import library.
// This import library will contain function signatures for the procedures in the DLL, so the linker can
// know what function takes which arguments and returs whatand where!!
