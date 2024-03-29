#include <stdio.h>
#include <Windows.h>

extern __declspec(dllimport) const char* __cdecl GreetUser(void);
extern __declspec(dllimport) const wchar_t* const name;

long double(__cdecl* fnptr)(long double); // CircleArea

int main(void) {
    printf_s("%s Anoban!\n", GreetUser());
    _putws(name);

    const HMODULE     DLLR = LoadLibraryExW(L"dllr.dll", NULL, 0);
    const long double area;

    fnptr = (long double(__cdecl*)(long double)) GetProcAddress(DLLR, "CircleArea");
    printf_s("Area of a circle with a diameter 12.5346545 cm is %.10Lf cm squares.\n", (*fnptr)(12.5346545 / 2));
    FreeLibrary(DLLR);
    return 0;
}
