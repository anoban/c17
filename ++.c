#include <stdio.h>

int main(void) {
    int i = 5;
    printf_s("%d\n", i);
    i = ++i + ++i;
    printf_s("%d\n", i);
    i = ++i + i++;
    printf_s("%d\n", i);
}