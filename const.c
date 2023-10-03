#include <stdio.h>

int main(void) {
    // array of arrays cannot be declared with double pointers!

    const char* names[] = { "James", "Janet", "Julian" };
    *names              = "james";

    // names[1][0] = 'j';

    char* const jobs[]  = { "Teacher", "Mason" };

    *jobs               = "teacher";
    jobs[1][0]          = 'm';
}