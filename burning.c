#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_MEAN
#ifdef __clang__
    #define UNICODE
#endif
// clang-format off
#include <Windows.h>
#include <CommCtrl.h>
// clang-format on

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Comctl32.lib")

HINSTANCE        hGlobalInstance;
LRESULT          rGlobalPosition = 200;

LRESULT CALLBACK PanelProcedure(_In_ HWND hWindow, _In_ UINT uMessage, _In_ WPARAM wParameter, _In_ LPARAM lParameter) {
    HBRUSH                      hYellowBrush, hRedBrush, hOldBrush;
    HPEN                        hPen, hOldPen;
    HFONT                       hFont, hOldFont;
    RECT                        rRect_0, rRect_1;
    PAINTSTRUCT                 psPainter;
    static const wchar_t* const lpswzCaptions[] = { L"75", L"150", L"225", L"300", L"375", L"450", L"525", L"600", L"675" };
    HDC                         hDeviceContext;
    int                         till = 0, step = 0, full = 0;

    switch (uMessage) {
        case WM_PAINT :
            hDeviceContext = BeginPaint(hWindow, &psPainter);
            GetClientRect(hWindow, &rRect_0);
            till         = rRect_0.right / 750.0L * rGlobalPosition;
            step         = rRect_0.right / 10.0L;
            full         = rRect_0.right / 750.0L * 700;

            hYellowBrush = CreateSolidBrush(RGB(255, 255, 184));
            hRedBrush    = CreateSolidBrush(RGB(255, 110, 110));
            hPen         = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
            hOldPen      = SelectObject(hDeviceContext, hPen);

            hFont        = CreateFontW(14, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0, 0, 0, 0, 0, L"Consolas");
            hOldFont     = SelectObject(hDeviceContext, hFont);

            if (till > full) {
                SelectObject(hDeviceContext, hYellowBrush);
                Rectangle(hDeviceContext, 0, 0, full, 30);
                hOldBrush = SelectObject(hDeviceContext, hRedBrush);
                Rectangle(hDeviceContext, full, 0, till, 0);
            } else {
                hOldBrush = SelectObject(hDeviceContext, hYellowBrush);
                Rectangle(hDeviceContext, 0, 0, till, 30);
            }

            SelectObject(hDeviceContext, hOldPen);
            for (long long i = 0; i < 10; ++i) {
                MoveToEx(hDeviceContext, i * step, 0, NULL);
                LineTo(hDeviceContext, i * step, 7);

                rRect_1.bottom = 28l;
                rRect_1.top    = 8;
                rRect_1.left   = i * step - 10;
                rRect_1.left   = i * step + 10;

                SetBkMode(hDeviceContext, TRANSPARENT);
                DrawTextW(hDeviceContext, lpswzCaptions[i - 1], wcsnlen_s(lpswzCaptions[i - 1], 5), &rRect_1, DT_CENTER);
            }

            SelectObject(hDeviceContext, hOldBrush);
            DeleteObject(hYellowBrush);
            DeleteObject(hRedBrush);

            DeleteObject(hPen);

            SelectObject(hDeviceContext, hOldFont);
            DeleteObject(hFont);

            EndPaint(hWindow, &psPainter);
            break;

        default : break;
    }

    return DefWindowProcW(hWindow, uMessage, wParameter, lParameter);
}

LRESULT CALLBACK WindowProcedure(_In_ HWND hWindow, _In_ UINT uMessage, _In_ WPARAM wParameter, _In_ LPARAM lParameter) {
    static HWND          hTrack, hBurn;
    WNDCLASSEXW          wcWindowClass;
    INITCOMMONCONTROLSEX icCtrlInit;

    wcWindowClass.cbSize = sizeof(WNDCLASSEXW);
    icCtrlInit.dwSize    = sizeof(INITCOMMONCONTROLSEX);
    icCtrlInit.dwICC     = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icCtrlInit);

    switch (uMessage) {
        case WM_CREATE :
            {
                wcWindowClass.lpszClassName = L"Burning Control";
                wcWindowClass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
                wcWindowClass.style         = CS_HREDRAW | CS_VREDRAW;
                wcWindowClass.hCursor       = LoadCursorW(NULL, IDC_ARROW);
                wcWindowClass.lpfnWndProc   = PanelProcedure;
                RegisterClassExW(&wcWindowClass);

                hBurn = CreateWindowExW(
                    WS_EX_STATICEDGE, L"Burning Control", NULL, WS_CHILD | WS_VISIBLE, 0, 330, 490, 30, hWindow, (HMENU) 1, NULL, NULL
                );

                hTrack = CreateWindowExW(
                    WS_EX_OVERLAPPEDWINDOW | WS_EX_STATICEDGE,
                    TRACKBAR_CLASSW,
                    NULL,
                    WS_CHILD | WS_VISIBLE | TBS_NOTICKS | TBS_FIXEDLENGTH,
                    40,
                    25,
                    150,
                    25,
                    hWindow,
                    (HMENU) 2,
                    hGlobalInstance,
                    NULL
                );

                SendMessageW(hTrack, TBM_SETRANGE, 1, MAKELONG(0, 750));
                SendMessageW(hTrack, TBM_SETPAGESIZE, 0, 20);
                SendMessageW(hTrack, TBM_SETTICFREQ, 20, 0);
                SendMessageW(hTrack, TBM_SETPOS, 1, 150);
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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pwszCommandLine, int iCmdShow) {
    HWND        hWindow;
    MSG         msg;
    WNDCLASSEXW wcexWindowdClass   = { 0 };

    wcexWindowdClass.cbSize        = sizeof(WNDCLASSEXW);
    wcexWindowdClass.lpszClassName = L"Application";
    wcexWindowdClass.hInstance     = hInstance;
    wcexWindowdClass.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wcexWindowdClass.lpfnWndProc   = WindowProcedure;
    wcexWindowdClass.hCursor       = LoadCursorW(0, IDC_ARROW);

    hGlobalInstance                = hInstance;

    RegisterClassExW(&wcexWindowdClass);
    hWindow = CreateWindowW(
        wcexWindowdClass.lpszClassName,
        L"Burning control",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
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