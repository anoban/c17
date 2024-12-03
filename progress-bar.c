#include <stdbool.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_MEAN
#include <Windows.h>

void static __forceinline ActivateVirtualTerminalEscapes(void) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        fprintf_s(
            stderr,
            "Error %lu in accessing STDOUT handle.\n\
            Activation of virtual terminal escapes failed!\n",
            GetLastError()
        );
    } else {
        DWORD dwStdOutMode = 0;
        if (!GetConsoleMode(hStdOut, &dwStdOutMode)) {
            fprintf_s(
                stderr,
                "Error %lu in call to GetConsoleMode().\n\
            Activation of virtual terminal escapes failed!\n",
                GetLastError()
            );
        } else {
            dwStdOutMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            if (!SetConsoleMode(hStdOut, dwStdOutMode)) {
                fprintf_s(
                    stderr,
                    "Error %lu in call to SetConsoleMode().\n\
            Activation of virtual terminal escapes failed!\n",
                    GetLastError()
                );
            }
        }
    }
    return;
}

void PrintProgress(LPDWORD lpdwCurrent, DWORD dwMaxLimit) {
    ActivateVirtualTerminalEscapes();

    // Has 100 -s.
    printf_s("\x1b[1m\x1b[35m----------------------------------------------------------------------------------------------------\x1b[0m\r"
    );
    while (*lpdwCurrent <= dwMaxLimit) {
        printf_s("\x1b[1m\x1b[32m>>");
        Sleep(100);
        printf_s("\b\b-");
        printf_s("\x1b[1m\x1b[0m");
    }

    // Print a yellow line once done.
    printf_s("\r");
    return;
}

int main(void) {
    DWORD dwMax = 1000, dwCaret = 50;
    while (dwCaret < dwMax) PrintProgress(&dwCaret, dwMax);
    // printf_s("\x1b[1m\x1b[46m\xE2\x9C\x93\x1b[0m\n");
    return 0;
}
