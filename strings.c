#include <stdio.h>
#include <string.h>

#pragma warning(disable : 4710)

int wmain(void) {
    // the const qualifier here applies only to the wide strings (wchar_t arrays) inside the array
    // NOT FOR THE ARRAY ITSELF
    // THE ARRAY IS STILL MUTABLE!
    const wchar_t* names[] = { L"Portman", L"Jimmy", L"Natalie", L"Julia", L"Roberts", L"Michelle", L"Jackman", L"Jason" };

    wprintf_s(L"sizeof(names) is %zu bytes!\n", sizeof names);
    // this will work!
    names[1] = L"Jean Jaques Rossaeu";

    // error l-value specifies const object
    // expression must be modifiable lvalue.
    // *(names[0]) = L"Emmanuel";

    wprintf_s(L"sizeof(names) is %zu bytes!\n", sizeof names);

    // sizeof names is 64 bytes, because it is just an array of 8 wchar_t pointers!
    // 8 * 8 = 64, what's so suprising huh?

    // Let's examine the addresses stored in names array
    for (size_t i = 0; i < 8; ++i) wprintf_s(L"%zu th element in names is stored @ %p (%zu)\n", i, names[i], names[i]);

    size_t len = 0;
    for (size_t i = 0; i < 8; ++i)
        wprintf_s(L"length of %ls is %zu bytes (size %zu bytes)\n", names[i], len = wcsnlen_s(names[i], 20 * sizeof(wchar_t)), len * 2);

    return 0;
}
