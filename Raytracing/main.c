#include "Util.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

BITMAPINFO canvas_bitmap_info;
HBITMAP canvas_bitmap;
HDC canvas_device_context;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR cmdLine, _In_ int showCmd)
{
    MSG msg = { 0 };
    HWND hwnd = { 0 };
    WNDCLASSW wc = { 0 };

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpszClassName = L"SAMPLE RAYTRACING";
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpszMenuName = NULL;
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassW(&wc);

    canvas_bitmap_info.bmiHeader.biSize = sizeof(canvas_bitmap_info.bmiHeader);
    canvas_bitmap_info.bmiHeader.biPlanes = 1;
    canvas_bitmap_info.bmiHeader.biBitCount = 32;
    canvas_bitmap_info.bmiHeader.biCompression = BI_RGB;
    canvas_device_context = CreateCompatibleDC(0);

    int posX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (WINDOW_WIDTH / 2);
    int posY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (WINDOW_HEIGHT / 2);

    hwnd = CreateWindowW(
        wc.lpszClassName,
        L"SAMPLE RAYTRACING",
        WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE,
        posX, posY,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    Render(hwnd);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC device_context;
            device_context = BeginPaint(hwnd, &paint);
            BitBlt(
                device_context,
                paint.rcPaint.left, paint.rcPaint.top,
                paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom - paint.rcPaint.top,
                canvas_device_context,
                paint.rcPaint.left, paint.rcPaint.top,
                SRCCOPY
            );
            EndPaint(hwnd, &paint);
        }
        break;
    case WM_SIZE:
        {
            canvas_bitmap_info.bmiHeader.biWidth = LOWORD(lParam);
            canvas_bitmap_info.bmiHeader.biHeight = -HIWORD(lParam);

            if (canvas_bitmap)
                DeleteObject(canvas_bitmap);
        
            canvas_bitmap = CreateDIBSection(NULL, &canvas_bitmap_info, DIB_RGB_COLORS, &canvas.pixels, 0, 0);
            SelectObject(canvas_device_context, canvas_bitmap);

            canvas.width = LOWORD(lParam);
            canvas.height = HIWORD(lParam);
        }
        break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}



