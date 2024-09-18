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

static_assert(offsetof(struct user, name) == 0);
static_assert(offsetof(struct user, occupation) == 40);
static_assert(offsetof(struct user, title) == 60);
static_assert(offsetof(struct user, age) == 72);
static_assert(offsetof(struct user, annual_income) == 80);
static_assert(offsetof(struct user, marital_status) == 88);
// static_assert(sizeof(user_t) == 96);

int wmain(void) {
    atomic_flag atflag = { false }; // ._Val is a MS implementation specific member designator

    _Atomic struct i32_array { // inline declaration
            int a[100];
    } arr;

    _Atomic struct pair {
            int x, y;
    } tuple;

    count = 1000;
    user_t jessie;

    wprintf_s(L"atomic_is_lock_free? %s\n", atomic_is_lock_free(&count) ? L"yes" : L"no");  // long long
    wprintf_s(L"atomic_is_lock_free? %s\n", atomic_is_lock_free(&jessie) ? L"yes" : L"no"); // user_t
    wprintf_s(L"atomic_is_lock_free? %s\n", atomic_is_lock_free(&arr) ? L"yes" : L"no");    // i32_array
    wprintf_s(L"atomic_is_lock_free? %s\n", atomic_is_lock_free(&tuple) ? L"yes" : L"no");  // pair

    return EXIT_SUCCESS;
}
