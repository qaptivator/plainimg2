#include <windows.h>

// Window procedure function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Drawing a single pixel at (100, 100) with RGB color (255, 0, 0) - Red
            SetPixel(hdc, 100, 100, RGB(255, 0, 0));

            EndPaint(hwnd, &ps);
        }
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int main()
{
    // Initialize the window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "SimpleWindowClass";

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        "Pixel Rendering Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
        NULL, NULL, wc.hInstance, NULL);

    if (hwnd == NULL)
    {
        return 0;
    }

    // Show and update the window
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
