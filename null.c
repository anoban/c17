#include <stdint.h>
#include <stdlib.h>

int main(void) {
    [[maybe_unused]] int64_t*    integerptr = (int64_t*) 4518789789;
    [[maybe_unused]] int64_t*    null       = NULL;
    [[maybe_unused]] int64_t     deref      = *integerptr;

    [[maybe_unused]] const void* ptr        = nullptr; // MSVC still doesn't support nullptr :(

    return EXIT_SUCCESS;
}
