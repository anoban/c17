#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_MEAN
#include <CommCtrl.h>
#include <Windows.h>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")

HINSTANCE        hGlobalInstance;
LRESULT          rGlobalPosition = 200;

LRESULT CALLBACK WindowProcedure(_In_ HWND hWindow, _In_ UINT uMessage, _In_ WPARAM wParameter, _In_ LPARAM lParameter) {
    static HWND          hTrack, hBurn;
    WNDCLASSEXW          wcWindowClass;
    INITCOMMONCONTROLSEX icCtrlInit;

    wcWindowClass.cbSize = sizeof(WNDCLASSEXW);
    icCtrlInit.dwSize    = sizeof(INITCOMMONCONTROLSEX);
    icCtrlInit.dwICC     = ICC_BAR_CLASSES;

    switch (uMessage) {
        case WM_CREATE :
            {
                wcWindowClass.lpszClassName = L"Burning Control";
                wcWindowClass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
                wcWindowClass.style         = CS_HREDRAW | CS_VREDRAW;
                wcWindowClass.hCursor       = LoadCursorW(NULL, IDC_ARROW);
                RegisterClassExW(&wcWindowClass);

                hBurn = CreateWindowExW(
                    WS_EX_STATICEDGE, L"Burning Control", NULL, WS_CHILD | WS_VISIBLE, 0, 330, 490, 30, hWindow, (HMENU) 1, NULL, NULL
                );

                hTrack = CreateWindowExW(
                    WS_EX_OVERLAPPEDWINDOW,
                    TRACKBAR_CLASSW,
                    NULL,
                    WS_CHILD | WS_VISIBLE | TBS_NOTICKS | TBS_FIXEDLENGTH,
                    40,
                    25,
                    152,
                    25,
                    hWindow,
                    (HMENU) 2,
                    hGlobalInstance,
                    NULL
                );

                SendMessageW(hTrack, TBM_SETRANGE, TRUE, MAKELONG(0, 750));
                SendMessageW(hTrack, TBM_SETPAGESIZE, 0, 20);
                SendMessageW(hTrack, TBM_SETTICFREQ, 20, 0);
                SendMessageW(hTrack, TBM_SETPOS, TRUE, 150);
                break;
            }
        case WM_SIZE :
            {
                SetWindowPos(hBurn, NULL, 0, HIWORD(lParameter) - 30, LOWORD(lParameter), 30, SWP_NOZORDER);
                break;
            }
        case WM_HSCROLL :
            {
                rGlobalPosition = SendMessageW(hTrack, TBM_GETPOS, 0, 0);
                InvalidateRect(hBurn, NULL, TRUE);
                break;
            }
        case WM_DESTROY :
            {
                PostQuitMessage(0);
                break;
            }
    }
    return DefWindowProcW(hWindow, uMessage, wParameter, lParameter);
}

LRESULT CALLBACK PanelProcedure(_In_ HWND hWindow, _In_ UINT uMessage, _In_ WPARAM wParameter, _In_ LPARAM lParameter) {
    HBRUSH                      hYellowBrush, hRedBrush, hOldBrush;
    HPEN                        hPen, hOldPen;
    HFONT                       hFont, hOldFont;
    RECT                        rRect_0, rRect_1;
    static const wchar_t* const lpswzCaptions[] = { L"75", L"150", L"225", L"300", L"375", L"450", L"525", L"600", L"675" };
    HDC                         hDeviceContext;
    int                         till = 0, step = 0, full = 0, i = 0;
}