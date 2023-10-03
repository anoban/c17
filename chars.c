#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFSIZE 1000U

int main(void) {
    char* buffer = NULL;

    buffer       = malloc(BUFFSIZE);
    assert(buffer);

    char* names[] = { NULL };
    for (int i = 0; i < BUFFSIZE / 20; ++i) {
        names[i] = buffer + (i * 20);
        if (i % 2)
            names[i] = "James";
        else
            names[i] = "Julia";
        printf_s("%X\n", names[i]);
        puts(names[i]);
    }

    int i = BUFFSIZE / 20;
    printf_s("i = %3d\n", i);

    do {
        // printf_s("%d bytes\n", names[i] - names[i - 1]);
        --i;
    } while (i > 1);

    free(buffer);

    return 0;
}