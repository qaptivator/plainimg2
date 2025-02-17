#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>

// Link with GDI+ library
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

// Global variables for GDI+ initialization and the image
ULONG_PTR gdiplusToken;
Image* pImage = NULL;

// Window procedure
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

            // Create a GDI+ graphics object for drawing
            Graphics graphics(hdc);

            // Draw the image on the window
            if (pImage != NULL) {
                graphics.DrawImage(pImage, 50, 50); // Drawing the image at (50, 50)
            }

            EndPaint(hwnd, &ps);
        }
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int main()
{
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Load the image file
    pImage = new Image(L"example.jpg"); // Change to your image file path

    // Initialize the window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "ImageWindowClass";

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        "Image Rendering Window",
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

    // Cleanup GDI+ resources
    if (pImage != NULL) {
        delete pImage;
    }
    GdiplusShutdown(gdiplusToken);

    return 0;
}
