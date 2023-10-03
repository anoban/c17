#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    srand(time(NULL));

    if (rand() % 2) goto LABEL;

    puts("rand() %% 2 == 0");

OUTERMOST:
    for (int i = 0; i < 100; ++i) {
MIDDLE:
        for (int j = 1000; j > 0; --j) {
            for (int k = 0; k > -1000; --k)
                if (k % 11) goto MIDDLE;
            if (j < 985) goto OUTERMOST;
        }
        if (i % 10) goto LABEL;
    }

LABEL:
    puts("Came here from GOTO!");

    puts("Still GOTO?");

    printf_s("A random number for you %8d\n", rand());

    {
        const int scoped_local = rand();
        printf_s("Scoped local random integer is %5d\n", scoped_local);
    }

    return 0;
}
