#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

struct align {
        bool               first;
        int16_t            second;
        double             third;
        char               fourth;
        unsigned long long fifth;
};

__declspec(align(32)) struct align_ {
        bool               first;
        int16_t            second;
        double             third;
        char               fourth;
        unsigned long long fifth;
};

int main(void) {
    alignof(struct align);
    return 0;
}
