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

HINSTANCE hGlobalInstance;
LRESULT   lGlobalPosition = 150;

LRESULT CALLBACK PanelProc(HWND hWindow, UINT uMessage, WPARAM wParameter, LPARAM lParameter) {
    HBRUSH      hYellowBrush, hRedBrush, hOldBrush;
    HPEN        hPen, hOldPen;
    HFONT       hFont, hOldFont;
    PAINTSTRUCT psPainter;
    RECT        rOne, rTwo;

    const WCHAR* pwszCaptions[] = { L"75", L"150", L"225", L"300", L"375", L"450", L"525", L"600", L"675" };
    HDC          hDeviceContext;
    INT          iTill = 0, iStep = 0, iFull = 0;

    switch (uMessage) {
        case WM_PAINT :
            hDeviceContext = BeginPaint(hWindow, &psPainter);
            GetClientRect(hWindow, &rOne);
            iTill        = rOne.right / 750 * lGlobalPosition;
            iStep        = rOne.right / 10;
            iFull        = rOne.right / 750 * 700;

            hYellowBrush = CreateSolidBrush(RGB(255, 255, 184));
            hRedBrush    = CreateSolidBrush(RGB(255, 110, 110));

            hPen         = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
            hOldPen      = SelectObject(hDeviceContext, hPen);

            hFont        = CreateFontW(13, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0, 0, 0, 0, 0, L"Tahoma");
            hOldFont     = SelectObject(hDeviceContext, hFont);

            if (iTill > iFull) {
                SelectObject(hDeviceContext, hYellowBrush);
                Rectangle(hDeviceContext, 0, 0, iFull, 30);
                hOldBrush = SelectObject(hDeviceContext, hRedBrush);
                Rectangle(hDeviceContext, iFull, 0, iTill, 30);
            } else {
                hOldBrush = SelectObject(hDeviceContext, hYellowBrush);
                Rectangle(hDeviceContext, 0, 0, iTill, 30);
            }

            SelectObject(hDeviceContext, hOldPen);
            for (size_t i = 1; i < 10; ++i) {
                MoveToEx(hDeviceContext, i * iStep, 0, NULL);
                LineTo(hDeviceContext, i * iStep, 7);
                ;
                rTwo.bottom = 28;
                rTwo.top    = 8;
                rTwo.left   = i * iStep - 10;
                rTwo.right  = i * iStep + 10;

                SetBkMode(hDeviceContext, TRANSPARENT);
                DrawTextW(hDeviceContext, pwszCaptions[i - 1], wcsnlen_s(pwszCaptions[i - 1], 4 /* in wchar_t s*/), &rOne, DT_CENTER);
            }

            SelectObject(hDeviceContext, hOldBrush);
            DeleteObject(hYellowBrush);
            DeleteObject(hRedBrush);
            DeleteObject(hPen);

            SelectObject(hDeviceContext, hOldFont);
            DeleteObject(hFont);

            EndPaint(hWindow, &psPainter);
            break;
    }
    return DefWindowProcW(hWindow, uMessage, wParameter, lParameter);
}

LRESULT CALLBACK WindowProc(HWND hWindow, UINT uMessage, WPARAM wParameter, LPARAM lParameter) {
    static HWND hWindowTrack, hWindowBurn;
    WNDCLASSEXW wcexWindowClass = { 0 };
    wcexWindowClass.cbSize      = sizeof(WNDCLASSEXW);

    INITCOMMONCONTROLSEX iccexExtControls;
    iccexExtControls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccexExtControls.dwICC  = ICC_BAR_CLASSES;
    InitCommonControlsEx(&iccexExtControls);

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

        case WM_SIZE : SetWindowPos(hWindowBurn, NULL, 0, HIWORD(lParameter) - 30, LOWORD(lParameter), 30, SWP_NOZORDER); break;

        case WM_HSCROLL :
            lGlobalPosition = SendMessageW(hWindowTrack, TBM_GETPOS, 0, 0);
            InvalidateRect(hWindowBurn, NULL, TRUE);
            break;

        case WM_DESTROY : PostQuitMessage(0); break;
    }
    return DefWindowProcW(hWindow, uMessage, wParameter, lParameter);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
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
