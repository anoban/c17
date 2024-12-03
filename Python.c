#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// A dummy Python.exe to test PyReleases.exe

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int minor = 0, major = 0;

    minor = (rand() % 5) + 5;
    major = (rand() % 9) + 1;

    if (!strcmp(argv[1], "--version")) printf_s("Python 3.%d.%d\n", major, minor);
    return 0;
}
