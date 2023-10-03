#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

typedef unsigned long long(__stdcall* fnptrFactorial)(int);

// Needs to load factorialDLL.dll

int main(void) {
    HMODULE hmDLL = LoadLibraryExW(L"./factorialDLL.dll", NULL, 0);
    if (!hmDLL) {
        fprintf_s(stderr, "Error %lu in LoadLibraryExW.\n", GetLastError());
        return 1;
    }

    fnptrFactorial fact = (fnptrFactorial) GetProcAddress(hmDLL, "Factorial");
    if (!fact) {
        fprintf_s(stderr, "Error %lu in GetProcAddress.\n", GetLastError());
        return 1;
    }

    int n = 14;

    printf_s("Factorial of %d is %llu\n", n, fact(n));
    return 0;
}