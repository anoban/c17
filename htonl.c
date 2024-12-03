#include <stdio.h>
#define _AMD64_
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_MEAN
#include <stdlib.h>
#include <winsock.h>

#pragma comment(lib, "Ws2_32.lib")

int main(void) {
    unsigned long number = 0x00AA00CC;
    wprintf_s(L"number: %u, htonl(number): %u\n", number, htonl(number));
    wprintf_s(L"number: %X, htonl(number): %X\n", number, htonl(number));
    return EXIT_SUCCESS;
}
