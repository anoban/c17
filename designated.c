// designated initializers

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct user {
        uint64_t    age;
        long double annual_salary;
        wchar_t*    job; // if declared as wchar_t job[compile time constant], post declaration assignments won't work!
        bool        ismarried;
} user_t;

int main(void) {
    // designated array initializers
    int64_t     numbers[10] = { [0] = 12, [5] = 20, [9] = 100 }; // elements that are not explicitly initialized will be zero initialized
    long double nombres[10] = { [0] = 1.234567, [9] = 2.345678 };
    for (size_t i = 0; i < __crt_countof(numbers); ++i)
        wprintf_s(L"numbers[%3zu] = %10lld, nombres[%3zu] = %10Lf\n", i, numbers[i], i, nombres[i]);

    // designated struct initializers.
    const user_t Jane = {
        .age = 34, .annual_salary = 12500.0564, .job = L"Accountant", .ismarried = true
    };                       // declaration and initialization at once => definition

    user_t John;             // declaration
    John.age           = 32; // member initialization
    John.annual_salary = 89200.764;
    John.ismarried     = false;
    John.job           = L"Opera singer";

    return 0;
}