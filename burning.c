// clang-format off
#define UNICODE
#include <wchar.h>
#include <windows.h>
#include <commctrl.h>
// clang-format on

#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "IPHlpApi.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK PanelProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hinst;
LRESULT   g_pos = 150;

int WINAPI
wWinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] PWSTR lpCmdLine, [[maybe_unused]] int nCmdShow) {
    [[maybe_unused]] HWND hwnd; // needs clang and -std=c23
    MSG                   msg;
    WNDCLASSW             wc = { 0 };

    wc.lpszClassName         = L"Application";
    wc.hInstance             = hInstance;
    wc.hbrBackground         = CreateSolidBrush(RGB(255, 0, 0)); // GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc           = WndProc;
    wc.hCursor               = LoadCursorW(0, IDC_ARROW);

    g_hinst                  = hInstance;

    RegisterClassW(&wc);
    hwnd = CreateWindowExW(
        0L,
        wc.lpszClassName,
        L"Burning control",
        (0x00000000L | 0x00C00000L | 0x00080000L | 0x00040000L | 0x00020000L | 0x00010000L) | 0x10000000L | 0x02000000L,
        100,
        100,
        400,
        250,
        0,
        0,
        hInstance,
        0
    );

    while (GetMessageW(&msg, NULL, 0, 0)) DispatchMessageW(&msg);

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hwndTrack_0, hwndTrack_1, hwndTrack_2, hwndBurn;
    WNDCLASSW   rwc = { 0 };

    INITCOMMONCONTROLSEX InitCtrlEx;

    InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCtrlEx.dwICC  = ICC_BAR_CLASSES;
    InitCommonControlsEx(&InitCtrlEx);

    switch (msg) {
        case WM_CREATE :

            rwc.lpszClassName = L"BurningControl";

            rwc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
            rwc.style         = CS_HREDRAW;
            rwc.lpfnWndProc   = PanelProc;
            rwc.hCursor       = LoadCursor(0, IDC_ARROW);
            RegisterClassW(&rwc);

            hwndBurn = CreateWindowExW(
                WS_EX_STATICEDGE, L"BurningControl", NULL, WS_CHILD | WS_VISIBLE, 0, 330, 490, 30, hwnd, (HMENU) 1, NULL, NULL
            );

            hwndTrack_0 = CreateWindowExW(
                0,
                TRACKBAR_CLASSW,
                NULL,
                WS_CHILD | WS_VISIBLE | TBS_FIXEDLENGTH | TBS_AUTOTICKS,
                40,
                25,
                150,
                25,
                hwnd,
                (HMENU) 2,
                g_hinst,
                NULL
            );

            hwndTrack_1 = CreateWindowExW(
                0,
                TRACKBAR_CLASSW,
                NULL,
                WS_CHILD | WS_VISIBLE | TBS_FIXEDLENGTH | TBS_AUTOTICKS,
                40,
                75,
                150,
                25,
                hwnd,
                (HMENU) 3,
                g_hinst,
                NULL
            );

            hwndTrack_2 = CreateWindowExW(
                0,
                TRACKBAR_CLASSW,
                NULL,
                WS_CHILD | WS_VISIBLE | TBS_FIXEDLENGTH | TBS_AUTOTICKS,
                40,
                125,
                150,
                25,
                hwnd,
                (HMENU) 4,
                g_hinst,
                NULL
            );

            SendMessageW(hwndTrack_0, TBM_SETRANGE, TRUE, MAKELONG(0, 750));
            SendMessageW(hwndTrack_0, TBM_SETPAGESIZE, 0, 20);
            SendMessageW(hwndTrack_0, TBM_SETTICFREQ, 20, 0);
            SendMessageW(hwndTrack_0, TBM_SETPOS, TRUE, 150);

            SendMessageW(hwndTrack_1, TBM_SETRANGE, TRUE, MAKELONG(0, 750));
            SendMessageW(hwndTrack_1, TBM_SETPAGESIZE, 0, 20);
            SendMessageW(hwndTrack_1, TBM_SETTICFREQ, 20, 0);
            SendMessageW(hwndTrack_1, TBM_SETPOS, TRUE, 150);

            SendMessageW(hwndTrack_2, TBM_SETRANGE, TRUE, MAKELONG(0, 750));
            SendMessageW(hwndTrack_2, TBM_SETPAGESIZE, 0, 20);
            SendMessageW(hwndTrack_2, TBM_SETTICFREQ, 20, 0);
            SendMessageW(hwndTrack_2, TBM_SETPOS, TRUE, 150);
            break;

        case WM_SIZE : SetWindowPos(hwndBurn, NULL, 0, HIWORD(lParam) - 30, LOWORD(lParam), 30, SWP_NOZORDER); break;

        case WM_HSCROLL :

            g_pos = SendMessageW(hwndTrack_0, TBM_GETPOS, 0, 0);
            InvalidateRect(hwndBurn, NULL, TRUE);
            break;

        case WM_DESTROY : PostQuitMessage(0); break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HBRUSH      hBrushYellow, hBrushRed, holdBrush;
    HPEN        hPen, holdPen;
    HFONT       hFont, holdFont;
    PAINTSTRUCT ps;
    RECT        rect, rect2;

    wchar_t* cap[] = { L"75", L"150", L"225", L"300", L"375", L"450", L"525", L"600", L"675" };

    HDC hdc;
    int till;
    int step, full;
    int i;

    switch (msg) {
        case WM_PAINT :

            hdc = BeginPaint(hwnd, &ps);

            GetClientRect(hwnd, &rect);

            till         = (rect.right / 750.0) * g_pos;
            step         = rect.right / 10.0;
            full         = (rect.right / 750.0) * 700;

            hBrushYellow = CreateSolidBrush(RGB(255, 255, 184));
            hBrushRed    = CreateSolidBrush(RGB(255, 110, 110));

            hPen         = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
            holdPen      = SelectObject(hdc, hPen);

            hFont        = CreateFontW(13, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0, 0, 0, 0, 0, L"Tahoma");

            holdFont     = SelectObject(hdc, hFont);

            if (till > full) {
                SelectObject(hdc, hBrushYellow);
                Rectangle(hdc, 0, 0, full, 30);
                holdBrush = SelectObject(hdc, hBrushRed);
                Rectangle(hdc, full, 0, till, 30);

            } else {
                holdBrush = SelectObject(hdc, hBrushYellow);
                Rectangle(hdc, 0, 0, till, 30);
            }

            SelectObject(hdc, holdPen);

            for (i = 1; i < 10; i++) {
                MoveToEx(hdc, i * step, 0, NULL);
                LineTo(hdc, i * step, 7);

                rect2.bottom = 28;
                rect2.top    = 8;
                rect2.left   = i * step - 10;
                rect2.right  = i * step + 10;

                SetBkMode(hdc, TRANSPARENT);
                DrawTextW(hdc, cap[i - 1], wcslen(cap[i - 1]), &rect2, DT_CENTER);
            }

            SelectObject(hdc, holdBrush);
            DeleteObject(hBrushYellow);
            DeleteObject(hBrushRed);

            DeleteObject(hPen);

            SelectObject(hdc, holdFont);
            DeleteObject(hFont);

            EndPaint(hwnd, &ps);
            break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}
