#include <stdio.h>

typedef unsigned int DWORD;
typedef void*        HANDLE;

int main(void) {
    DWORD STD_INPUT_HANDLE  = -10;
    DWORD STD_OUTPUT_HANDLE = -11;
    DWORD STD_ERROR_HANDLE  = -12;

    printf(
        "STD_INPUT_HANDLE = %lu, STD_OUTPUT_HANDLE = %lu & STD_ERROR_HANDLE = %lu\n", STD_INPUT_HANDLE, STD_OUTPUT_HANDLE, STD_ERROR_HANDLE
    );

    HANDLE INVALID_HANDLE_VALUE = -1;
    return 0;
}