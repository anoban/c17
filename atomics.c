#include <assert.h>
#include <stdalign.h>
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

typedef _Atomic(struct user {
    char     name[MAXCHARS_NAME];
    char     occupation[MAXCHARS_OCCUPATION];
    char     title[MAXCHARS_TITLE];
    // 2 bytes padding here
    unsigned age;
    // 4 bytes padding here
    double   annual_income;
    bool     marital_status;
    // 7 bytes padding here
}) user_t;

static_assert(offsetof(struct user, name) == 0);
static_assert(offsetof(struct user, occupation) == 40);
static_assert(offsetof(struct user, title) == 60);
static_assert(offsetof(struct user, age) == 72);
static_assert(offsetof(struct user, annual_income) == 80);
static_assert(offsetof(struct user, marital_status) == 88);
// static_assert(sizeof(user_t) == 96);

int wmain(void) {
    [[maybe_unused]] atomic_flag atflag = { false }; // ._Val is a MS implementation specific member designator

    // inline anonymous struct
    _Atomic(struct { int a[100]; }) array;

    _Atomic(struct { int x, y; }) ipair;

    count = 1000;
    user_t jessie;

    wprintf_s(L"atomic_is_lock_free? %s\n", atomic_is_lock_free(&count) ? L"yes" : L"no"); // long long
    wprintf_s(L"atomic_is_lock_free? %s\n", atomic_is_lock_free(&ipair) ? L"yes" : L"no");
    wprintf_s(L"atomic_is_lock_free? %s\n", atomic_is_lock_free(&jessie) ? L"yes" : L"no"); // user_t
    wprintf_s(L"atomic_is_lock_free? %s\n", atomic_is_lock_free(&array) ? L"yes" : L"no");

    // this is how LLVM defines the atomic_is_lock_free macro
    // #define atomic_is_lock_free(obj) __c11_atomic_is_lock_free(sizeof(*(obj)))

    // this is how MSVC defines the atomic_is_lock_free macro
    // #define atomic_is_lock_free(_Obj) _Atomic_is_lock_free(sizeof(__typeof_unqual__(*(_Obj))))

    // inline _Bool _Atomic_is_lock_free(size_t _Sz) {
    //    return _Sz <= 8 && (_Sz & _Sz - 1) == 0;
    // }

    // one requirement is that the type must be equal to or smaller than 8 bytes
    // the second requirement is that the size must be in the powers of 2

    const unsigned size = 0b0110 & 0b0110 - 1; // 0b0110 & 0b0101 = 0b0100  WRONG
    0b0100 & 0b0100 - 1;                       // 0b0100 & 0b0011 = 0b0000
    0b0010 & 0b0010 - 1;                       // 0b0010 & 0b0001 = 0b0000
    0b0011 & 0b0011 - 1;                       // 0b0011 & 0b0010 = 0b0010  WRONG
    0b0111 & 0b0111 - 1;                       // 0b0111 & 0b0110 = 0b0110  WRONG
    0b0001 & 0b0001 - 1;                       // 0b0001 & 0b0000 = 0b0000

#define atomic_is_lock_free_impl(sz) (((sz) <= 8) && !((sz) & ((sz) - 1)))

    static_assert(atomic_is_lock_free_impl(sizeof(char)));
    static_assert(atomic_is_lock_free_impl(sizeof(unsigned short)));
    static_assert(atomic_is_lock_free_impl(sizeof(unsigned)));
    static_assert(atomic_is_lock_free_impl(sizeof(long)));
    static_assert(atomic_is_lock_free_impl(sizeof(float)));
    static_assert(atomic_is_lock_free_impl(sizeof(long long)));
    static_assert(atomic_is_lock_free_impl(sizeof(double)));
    static_assert(!atomic_is_lock_free_impl(sizeof(user_t)));

    _Atomic(struct {
        double        price;
        unsigned char age;
    }) dpair;

#pragma pack(push, 1)
    _Atomic(/* alignas(1) */ struct name { unsigned char alphabets[9]; });
#pragma pack(pop)

    static_assert(!atomic_is_lock_free_impl(sizeof dpair));
    static_assert(!atomic_is_lock_free_impl(sizeof(struct name))); // 9 bytes with #pragma pack()

    // with LLVM based compilers we run into lld-link: error: undefined symbol: __atomic_is_lock_free
    // looks like LLVM doesn't support <stdatomic.h> fully on Windows since MSVC's support for <stdatomic.h> is still experimental
    // read up https://discourse.llvm.org/t/improve-autolinking-of-compiler-rt-and-libc-on-windows-with-lld-link/71392/2 for peripheral info
    return EXIT_SUCCESS;
}
