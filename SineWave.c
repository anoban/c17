/*-----------------------------------------
   SINEWAVE.C -- Sine Wave Using Polyline
                 (c) Charles Petzold, 1998
  -----------------------------------------*/

#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "User32.lib")

#define NUM   10000
#define TWOPI (2.0L * M_PI)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    static WCHAR szAppName[] = L"SineWave";
    HWND         hwnd;
    MSG          msg;
    WNDCLASSEXW  wndclass;

    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClassExW(&wndclass)) {
        MessageBoxW(NULL, L"Program requires Windows NT!", szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindowExW(
        0,
        szAppName,
        L"Sine Wave Using Polyline",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return EXIT_SUCCESS;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static int  cxClient, cyClient;
    HDC         hdc;
    int         i;
    PAINTSTRUCT ps;
    POINT       apt[NUM];

    switch (message) {
        case WM_SIZE :
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            return 0;

        case WM_PAINT :
            hdc = BeginPaint(hwnd, &ps);

            MoveToEx(hdc, 0, cyClient / 2, NULL);
            LineTo(hdc, cxClient, cyClient / 2);

            for (i = 0; i < NUM; i++) {
                apt[i].x = i * cxClient / NUM;
                apt[i].y = (int) (cyClient / 2 * (1 - sin(TWOPI * i / NUM)));
            }

            Polyline(hdc, apt, NUM);
            return 0;

        case WM_DESTROY : PostQuitMessage(0); return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
