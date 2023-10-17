#include <stdlib.h>

int main(void) {
    void* ptr = malloc(100);
    free(ptr);
    return EXIT_SUCCESS;
}