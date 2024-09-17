#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// use the compiler flag /experimental:c11atomics for MSVC
// atomics are useful in preventing coalescing/concurrent reads and writes
// when a value is being atomically read or overwritten, another operation cannot be performed on that value
// this is regulated at the hardware level with only partial involvement of software level constructs like locks or mutexes that provide controlled
// acquire and release mechanisms to resources accessed in a multithreaded context
// atomic operations are helpful in preventing dead locks and race conditions

#define MAXCHARS_NAME       40LLU
#define MAXCHARS_OCCUPATION 20LLU
#define MAXCHARS_TITLE      10LLU

atomic_llong count = 0;

typedef _Atomic struct user {
        char     name[MAXCHARS_NAME];
        char     occupation[MAXCHARS_OCCUPATION];
        char     title[MAXCHARS_TITLE];
        // 2 bytes padding here
        unsigned age;
        // 4 bytes padding here
        double   annual_income;
        bool     marital_status;
        // 7 bytes padding here
} user_t;

static_assert(sizeof(user_t) == 96);

int wmain(void) {
    atomic_flag atflag = { false }; // ._Val is a MS implementation specific member designator

    _Atomic struct A { // inline declaration
            int a[100];
    } a = { 0 };

    _Atomic struct B {
            int x, y;
    } b;

    count = 1000;
    wprintf_s(L"atomic_is_lock_free? %s\n", atomic_is_lock_free(&count) ? L"yes" : L"no");
    wprintf_s(L"atomic_is_lock_free? %s\n", atomic_is_lock_free(&a) ? L"yes" : L"no");
    wprintf_s(L"atomic_is_lock_free? %s\n", atomic_is_lock_free(&b) ? L"yes" : L"no");
    return EXIT_SUCCESS;
}
