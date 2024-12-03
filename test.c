#include <stdio.h>

int main(void) {
    int offset     = 0;
    int array[100] = { 0 };

    for (int i = 0; i < 100; ++i) {
        if (i % 2) {
            // This scope will be executed only when i is an odd number.
            array[offset++] = i;
            // offset++;
            printf_s("i = %3d offset has been incremented by one, offset = %3d\n", i, offset);
            printf_s("The odd number has been stored in an array, %3d\n", array[offset - 1]);
            // offset - 1 since we incremented the offset after storing the odd number,
            // now offset will be pointing to a zero initialized empty slot.
        }
    }

    puts("Printing array");
    for (int i = 0; i < 100; ++i) printf_s("%3d ", array[i]);
}
