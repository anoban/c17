#include <stdio.h>

int main(void) {
    int x = 0;
    int y = 100;
    x     = (x < y) ? y : x;

    return 0;
}