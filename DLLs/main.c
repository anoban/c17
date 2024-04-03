#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

// prototype for DLL routines, needed for load time linking
extern long double __declspec(dllexport) __cdecl factorial(unsigned n);
extern unsigned long long __stdcall gcd(const unsigned long long a, const unsigned long long b);
extern const wchar_t* const greeting;
extern const char* const    name;

int                         main(void) {
    // load time linking
    const unsigned x = 5;

    wprintf_s(L"Factorial of %4u is %4.5Lf\n", x, factorial(x));
    const unsigned long long a = 200, b = 40;
    wprintf(L"gcd of %llu and %llu is %llu\n", a, b, gcd(a, b));

    // runtime linking
    // for runtime linking, one does not need to link the import libraries.
    const HMODULE hDLL                                   = LoadLibraryExW(L"./dynamic.dll", NULL, 0);
    long double(__cdecl* const ptrfactorial)(unsigned n) = GetProcAddress(hDLL, "factorial");
    unsigned long long(__stdcall* const ptrgcd)(unsigned long long, unsigned long long) = GetProcAddress(hDLL, "gcd");

    // GetProcAddress basically does a series of strcmp searches on the DLL to find the requested procedure!

    if (ptrfactorial) wprintf_s(L"Factorial of %4u is %4.5Lf\n", x, (*ptrfactorial)(x));
    if (ptrgcd) wprintf(L"gcd of %llu and %llu is %llu\n", a, b, (*ptrgcd)(a, b));

    _putws(greeting); // relies on load time linking
    puts(name);       // relies on load time linking

    FreeLibrary(hDLL);
    return EXIT_SUCCESS;
}
