#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_MEAN
#ifdef __clang__
    #define UNICODE
#endif

// clang-format off
#include <stdlib.h>
#include <Windows.h>
#include <CommCtrl.h>   // must be included after <Windows.h>
// clang-format on

#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Comctl32.lib")

static HINSTANCE hGlobalInstance;
static HBRUSH    hOldBrush;
static HDC       hDeviceContext;

LRESULT CALLBACK WindowProc(HWND hWindow, UINT uMessage, WPARAM wParameter, LPARAM lParameter) {
    static HWND                       hWindow;
    static const INITCOMMONCONTROLSEX commCtrls = { .dwSize = sizeof(INITCOMMONCONTROLSEX), .dwICC = ICC_WIN95_CLASSES };
    InitCommonControlsEx(&commCtrls);

    switch (uMessage) {
        case WM_CREATE :
            wcexWindowClass.lpszClassName = L"BurningControl";
            wcexWindowClass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
            wcexWindowClass.style         = CS_HREDRAW;
            wcexWindowClass.lpfnWndProc   = PanelProc;
            wcexWindowClass.hCursor       = LoadCursorW(NULL, IDC_ARROW);
            RegisterClassExW(&wcexWindowClass);

            hWindowBurn = CreateWindowExW(
                WS_EX_STATICEDGE, L"BurningControl", NULL, WS_CHILD | WS_VISIBLE, 40, 25, 150, 25, hWindow, (HMENU) 2, NULL, NULL
            );

            hWindowTrack = CreateWindowExW(
                0,
                TRACKBAR_CLASSW,
                NULL,
                WS_CHILD | WS_VISIBLE | TBS_FIXEDLENGTH | TBS_NOTICKS,
                0,
                330,
                490,
                30,
                hWindow,
                (HMENU) 1,
                hGlobalInstance,
                NULL
            );

            SendMessageW(hWindowTrack, TBM_SETRANGE, 1, MAKELONG(0, 750));
            SendMessageW(hWindowTrack, TBM_SETPAGESIZE, 0, 20);
            SendMessageW(hWindowTrack, TBM_SETTICFREQ, 20, 0);
            SendMessageW(hWindowTrack, TBM_SETPOS, 1, 150);
            break;

        case WM_DESTROY : PostQuitMessage(0); break;
        default         : break;
    }
    return DefWindowProcW(hWindow, uMessage, wParameter, lParameter);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    const WCHAR szwApplication[] = L"Custom Control";
    HWND        hWindow;
    MSG         mMessage;
    WNDCLASSEXW wcexWindowClass   = { 0 };

    wcexWindowClass.cbSize        = sizeof(WNDCLASSEXW);
    wcexWindowClass.style         = CS_HREDRAW | CS_VREDRAW;
    wcexWindowClass.lpfnWndProc   = WindowProc;
    wcexWindowClass.cbClsExtra    = 0;
    wcexWindowClass.cbWndExtra    = 0;
    wcexWindowClass.hInstance     = hInstance;
    wcexWindowClass.hIcon         = LoadIconW(NULL, IDI_APPLICATION);
    wcexWindowClass.hCursor       = LoadCursorW(NULL, IDC_ARROW);
    wcexWindowClass.hbrBackground = (HBRUSH) GetSysColorBrush(COLOR_3DFACE);
    wcexWindowClass.lpszMenuName  = NULL;
    wcexWindowClass.lpszClassName = szwApplication;

    if (!RegisterClassExW(&wcexWindowClass)) {
        MessageBoxW(NULL, L"Program requires Windows NT!", szwApplication, MB_ICONERROR);
        return EXIT_FAILURE;
    }

    hWindow = CreateWindowExW(
        0,
        szwApplication,
        L"Burning Control",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
        100,
        100,
        400,
        250,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hWindow, iCmdShow);
    UpdateWindow(hWindow);

    while (GetMessageW(&mMessage, NULL, 0, 0)) {
        TranslateMessage(&mMessage);
        DispatchMessageW(&mMessage);
    }

    return EXIT_SUCCESS;
}
