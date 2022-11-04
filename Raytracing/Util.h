#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <Windows.h>

const wchar_t CLASS_NAME[] = L"SAMPLE RAYTRACING";

const unsigned int WINDOW_SIZE_X = 600;
const unsigned int WINDOW_SIZE_Y = 600;

typedef struct Sphere
{
	int center[3];
	int radius;
	int color[3];
} Sphere;

void DrawSphere(HWND hwnd, RECT rect, Sphere s)
{
	PAINTSTRUCT ps;

	HBRUSH hBrush = CreateSolidBrush(
		RGB(
			s.color[0],
			s.color[1],
			s.color[2]
		)
	);
	HPEN hPen = CreatePen(PS_NULL, 0, NULL);

	BeginPaint(hwnd, &ps);
	
	SelectObject(ps.hdc, hBrush);
	SelectObject(ps.hdc, hPen);

	Ellipse(
		ps.hdc,
		s.radius + 10,
		s.radius + 10,
		rect.right - 10,
		rect.bottom - 10
	);

	EndPaint(hwnd, &ps);
}

void PutPixel(HWND hwnd, RECT rect, int x, int y, COLORREF color)
{
	PAINTSTRUCT ps;

	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN hPen = CreatePen(PS_NULL, 0, NULL);

	BeginPaint(hwnd, &ps);
	
	SelectObject(ps.hdc, hBrush);
	SelectObject(ps.hdc, hPen);
	
	Rectangle(
		ps.hdc,
		x, y,
		x + 1, y + 1
	);

	EndPaint(hwnd, &ps);
}

#endif
