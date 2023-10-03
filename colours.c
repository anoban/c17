#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <stdio.h>
#include <Windows.h>

BOOL __stdcall __declspec(dllexport) ActivateVirtualTerminalEscapes(VOID) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        fprintf_s(stderr, "Error %ld in getting hConsole handle.\n", GetLastError());
        return FALSE;
    }
    DWORD dwMode = 0;
    if (!GetConsoleMode(hConsole, &dwMode)) {
        fprintf_s(stderr, "Error %ld in getting console mode.\n", GetLastError());
        return FALSE;
    }
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hConsole, dwMode)) {
        fprintf_s(stderr, "Error %ld in enabling virtual terminal escapes.\n", GetLastError());
        return FALSE;
    }
    return TRUE;
}