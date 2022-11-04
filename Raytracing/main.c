#include <stdio.h>
#include <Windows.h>

#pragma comment(lib, "user32.lib")

const wchar_t CLASS_NAME[] = L"SAMPLE RAYTRACING";

const unsigned int WINDOW_SIZE_X = 600;
const unsigned int WINDOW_SIZE_Y = 600;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR cmdLine, _In_ int showCmd)
{
    MSG msg = { 0 };
    HWND hwnd = { 0 };
    WNDCLASSW wc = { 0 };

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpszClassName = CLASS_NAME;
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpszMenuName = NULL;
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassW(&wc);

    int posX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (WINDOW_SIZE_X / 2);
    int posY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (WINDOW_SIZE_Y / 2);

    hwnd = CreateWindowW(
        wc.lpszClassName,
        CLASS_NAME,
        WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE,
        posX, posY,
        WINDOW_SIZE_X, WINDOW_SIZE_Y,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hwnd, showCmd);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    static RECT rcClient;

    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
    HPEN hPen = CreatePen(PS_NULL, 0, NULL);

    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        BeginPaint(hwnd, &ps);
        SelectObject(ps.hdc, hBrush);
        SelectObject(ps.hdc, hPen);
        Ellipse(
            ps.hdc,
            10,
            10,
            rcClient.right - 10,
            rcClient.bottom - 10
        );
        EndPaint(hwnd, &ps);
        break;
    case WM_SIZE:
        GetClientRect(hwnd, &rcClient);
        break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}



