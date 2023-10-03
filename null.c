#include <stdint.h>

int main(void) {
    int64_t* integerptr = (int64_t*) 4518789789;
    int64_t* nullptr    = NULL;
    int64_t deref       = *integerptr;
    return 0;
}