#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

typedef enum days_t { MONDAY = 1, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY } days_t;

static const wchar_t* const _wdays[] = {
    L"MONDAY", L"TUESDAY", L"WEDNESDAY", L"THURSDAY", L"FRIDAY", L"SATURDAY", L"SUNDAY",
};

int wmain(void) {
    printf_s("%d\n", SUNDAY);

    for (days_t i = 0; i < 12; ++i) wprintf_s(L"%s\n", _wdays[i]); // access violation
    // ++ does not do any wrap arounds after 7 like the overloaded prefixed ++ operatorin C++
    // once i goes above 6, we'll have an access violation exception
    return EXIT_SUCCESS;
}
