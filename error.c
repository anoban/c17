#include <master.h>

// A convenient wrapper for Win32 graphical error messages. failed_proc name must be less than 100 wchars.
void imerror_show(_In_ const wchar_t* restrict failed_proc) {
    wchar_t buffer[150]       = { 0 };
    wchar_t final_buffer[350] = { 0 };
    // https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessage
    FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
        buffer,
        150U,
        NULL
    );

    swprintf_s(final_buffer, 350U, L"Error %lu occured in call to %ls\n%ls\n", GetLastError(), failed_proc, buffer);
    // hWnd can be NULL if the message box has no other windows.
    // lpCaption can

    int resp = MessageBoxExW(NULL, final_buffer, failed_proc, MB_ICONERROR | MB_OK, MAKELANGID(SUBLANG_DEFAULT, SUBLANG_SYS_DEFAULT));
    if (resp == IDOK) return;
}

// a wrapper that shows a plain string as a popup.
// no internal error string lookups involved, whatsoever.
void display_message(_In_ const wchar_t* restrict msg) {
    if (IDOK == MessageBoxExW(NULL, msg, L"Error", MB_ICONERROR | MB_OK, MAKELANGID(SUBLANG_DEFAULT, SUBLANG_SYS_DEFAULT))) return;
}
