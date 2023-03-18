#ifndef UTIL_H
#define UTIL_H

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <Windows.h>

#include "MathHelper.h"

const wchar_t CLASS_NAME[] = L"SAMPLE RAYTRACING";

const unsigned int PROJECTION_PLANE_D = 1;

const unsigned int VIEWPORT_WIDTH = 1;
const unsigned int VIEWPORT_HEIGHT = 1;

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

struct {
	int width;
	int height;
	uint32_t* pixels;
} canvas = { 0 };

typedef struct Color
{
	int r;
	int g;
	int b;
} Color;

typedef struct Sphere
{
	Vector3D center;
	int radius;
	Color color;
} Sphere;

const Color BACKGROUND_COLOR = { 255, 255, 255 };

const Sphere scene[3] = {
	{.center = {.x = 0.0f, .y = -1.0f, .z = 3.0f}, .radius = 1, .color = {.r = 255, .g = 0, .b = 0} },
	{.center = {.x = 2.0f, .y = 0.0f, .z = 4.0f}, .radius = 1, .color = {.r = 0, .g = 0, .b = 255} },
	{.center = {.x = -2.0f, .y = 0.0f, .z = 4.0f}, .radius = 1, .color = {.r = 0, .g = 255, .b = 0} }
};

void Render();
Vector3D CanvasToViewport(int x, int y);
Color TraceRay(Vector3D O, Vector3D D, float t_min, float t_max);
Array2D IntersectRaySphere(Vector3D O, Vector3D D, Sphere sphere);
bool SphereIsNull(Sphere sphere);

void PutPixel(int x, int y, Color color)
{
	x = (canvas.width / 2) + x;
	y = (canvas.height / 2) - y - 1;

	if (x < 0 || x >= canvas.width || y < 0 || y >= canvas.height)
		return;

	canvas.pixels[(y * canvas.width) + x] = RGB(color.b, color.g, color.r);
}

void Render()
{
	Vector3D O = { 0.0f, 0.0f, 0.0f };

	for (int x = -canvas.width / 2; x < canvas.width / 2; x++)
	{
		for (int y = -canvas.height / 2; y < canvas.height / 2; y++)
		{
			Vector3D D = CanvasToViewport(x, y);
			Color color = TraceRay(O, D, 1, FLT_MAX);
			PutPixel(x, y, color);
		}
	}
}

Vector3D CanvasToViewport(int x, int y)
{
	Vector3D result = {
		x * (float)VIEWPORT_WIDTH / (float)WINDOW_WIDTH,
		y * (float)VIEWPORT_HEIGHT / (float)WINDOW_HEIGHT,
		(float)PROJECTION_PLANE_D
	};
	
	return result;
}

Color TraceRay(Vector3D O, Vector3D D, float t_min, float t_max)
{
	float closest_t = FLT_MAX;
	Sphere closestSphere = { 0 };

	for (int i = 0; i < 3; i++)
	{
		Array2D t = IntersectRaySphere(O, D, scene[i]);

		if ((t.a <= t_max && t.a >= t_min) && t.a < closest_t)
		{
			closest_t = t.a;
			closestSphere = scene[i];
		}

		if ((t.b <= t_max && t.b >= t_min) && t.b < closest_t)
		{
			closest_t = t.b;
			closestSphere = scene[i];
		}
	}

	if (SphereIsNull(closestSphere))
		return BACKGROUND_COLOR;

	return closestSphere.color;
}

Array2D IntersectRaySphere(Vector3D O, Vector3D D, Sphere sphere)
{
	int r = sphere.radius;
	Vector3D CO = VectorSubstract(O, sphere.center);

	float a = DotProduct(D, D);
	float b = 2 * DotProduct(CO, D);
	float c = DotProduct(CO, CO) - r * r;

	float discriminant = b * b - 4 * a * c;

	if (discriminant < 0)
	{
		Array2D result = { FLT_MAX, FLT_MAX };
		return result;
	}

	float t1 = (-b + sqrt(discriminant)) / (2 * a);
	float t2 = (-b - sqrt(discriminant)) / (2 * a);

	Array2D result = { t1, t2 };
	return result;
}

bool SphereIsNull(Sphere sphere)
{
	bool centerIsNull = sphere.center.x == 0.0f && sphere.center.y == 0.0f && sphere.center.z == 0.0f;
	bool colorIsNull = sphere.color.r == 0 && sphere.color.g == 0 && sphere.color.b == 0;

	if (centerIsNull && sphere.radius == 0 && colorIsNull)
		return true;

	return false;
}

#endif
