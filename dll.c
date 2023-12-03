// for launch time loading

#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_MEAN
#include <Windows.h>

// A major disdvantage of using .DLLs is we cannot do LTO when dlls are involved.

__declspec(dllexport) const char* __cdecl GreetUser(void) { return "Hi there!"; }

__declspec(dllexport) const wchar_t* const name = L"Julia Mitchelle";