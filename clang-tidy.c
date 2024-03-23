#include <math.h>
#include <stdint.h>
#include <stdio.h>

long long mul(int a, int b) {
    return a * b;
    // warning: performing an implicit widening conversion to type 'long' of a multiplication performed in type 'int'
}

char* ptr_add(char* base, int a, int b) {
    return base + a * b;
    // warning: result of multiplication in type 'int' is used as a pointer offset after an implicit widening conversion to type 'ssize_t'
}

char ptr_subscript(char* base, int a, int b) {
    return base[a * b];
    // warning: result of multiplication in type 'int' is used as a pointer offset after an implicit widening conversion to type 'ssize_t'
}

void zeroinit(char* base, unsigned width, unsigned height) {
    for (unsigned row = 0; row != height; ++row) {
        for (unsigned col = 0; col != width; ++col) {
            char* ptr = base + row * width + col;
            *ptr      = 0;
        }
    }
}

int main(void) {
    uint16_t bugprone_assignment_inside_if = 0;
    if (bugprone_assignment_inside_if = sizeof(long)) bugprone_assignment_inside_if *= 8;
    if ((bugprone_assignment_inside_if = sizeof(double))) bugprone_assignment_inside_if *= 4;

    const bool* boolptr = NULL; // won't probably work for C, since C doesn't have a true bool type :(
    if (boolptr) _putws(L"misleading");
    // this will be flagged

    char                   var = 'A';
    const uintptr_t* const ptr = &var;
    if (ptr) _putws(L"whoops"); // is okay

    long double  x   = powl(/*_X=*/12.9759475, /*_Y=*/4.6424);

    double       xx  = powl(/*X=*/12.9759475, /*Y_=*/4.6424);

    const double xxx = powl(/*_X=*/12.9759475, /*_y=*/2.000);

    if (var) {
        x  = 34;
        xx = 767;
    } else {
        x  = 34;
        xx = 767;
    }

    return 0;
}
