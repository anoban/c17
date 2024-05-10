#include <stdint.h>
#include <Windows.h>

int wmain(void) {
    uint64_t sum = 0;
    for (size_t i = 0; i < 100; ++i) sum += i;

    ExitProcess(11);
}
