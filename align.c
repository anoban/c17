#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

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
    wprintf_s(L"Hi there %s\n", L"Anoban");
    return 0;
}
