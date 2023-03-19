#ifndef UTIL_H
#define UTIL_H

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <Windows.h>

#include "LightHelper.h"
#include "MathHelper.h"

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
	int specular;
} Sphere;

#define PROJECTION_PLANE_D 1

#define VIEWPORT_WIDTH 1
#define VIEWPORT_HEIGHT 1

#define CANVAS_WIDTH 600
#define CANVAS_HEIGHT 600

#define BACKGROUND_COLOR (Color){ 0, 0, 0 }

#define SPHERES 4

static const Sphere SCENE_S[SPHERES] = {
	{.center = {.x = 0.0f, .y = -1.0f, .z = 3.0f}, .radius = 1, .color = {.r = 255, .g = 0, .b = 0}, .specular = 500},
	{.center = {.x = 2.0f, .y = 0.0f, .z = 4.0f}, .radius = 1, .color = {.r = 0, .g = 0, .b = 255}, .specular = 500},
	{.center = {.x = -2.0f, .y = 0.0f, .z = 4.0f}, .radius = 1, .color = {.r = 0, .g = 255, .b = 0}, .specular = 10},
	{.center = {.x = 0.0f, .y = -5001.0f, .z = 0.0f}, .radius = 5000, .color = {.r = 255, .g = 255, .b = 0}, .specular = 1000}
};

static const Light SCENE_L[LIGHTS] = {
	{.type = AMBIENT_LIGHT, .intensity = 0.2f, .coords = {.x = 0.0f, .y = 0.0f, .z = 0.0f}},
	{.type = POINT_LIGHT, .intensity = 0.6f, .coords = {.x = 2.0f, .y = 1.0f, .z = 0.0f}},
	{.type = DIRECTIONAL_LIGHT, .intensity = 0.2f, .coords = {.x = 1.0f, .y = 4.0f, .z = 4.0f}}
};

/* Functions prototypes */
void PutPixel(int x, int y, Color color);
void Render();
Vector3D CanvasToViewport(int x, int y);
Color TraceRay(Vector3D O, Vector3D D, float t_min, float t_max);
Array2D IntersectRaySphere(Vector3D O, Vector3D D, Sphere sphere);
bool SphereIsNull(Sphere sphere);
float ComputeLighting(Vector3D point, Vector3D normal);
/* */

/* Actual functions */
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
	Vector3D O = { 0.0f, 0.0f, -0.15f };

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
		x * (float)VIEWPORT_WIDTH / (float)CANVAS_WIDTH,
		y * (float)VIEWPORT_HEIGHT / (float)CANVAS_HEIGHT,
		(float)PROJECTION_PLANE_D
	};
	
	return result;
}

Color TraceRay(Vector3D O, Vector3D D, float t_min, float t_max)
{
	float closest_t = FLT_MAX;
	Sphere closestSphere = { 0 };

	for (int i = 0; i < SPHERES; i++)
	{
		Array2D t = IntersectRaySphere(O, D, SCENE_S[i]);

		if ((t.a <= t_max && t.a >= t_min) && t.a < closest_t)
		{
			closest_t = t.a;
			closestSphere = SCENE_S[i];
		}

		if ((t.b <= t_max && t.b >= t_min) && t.b < closest_t)
		{
			closest_t = t.b;
			closestSphere = SCENE_S[i];
		}
	}

	if (SphereIsNull(closestSphere))
		return BACKGROUND_COLOR;

	Vector3D P = VectorAdd(O, ScalarMul(closest_t, D));
	Vector3D N = VectorSubstract(P, closestSphere.center);
	N = ScalarMul(1 / Length(N), N);

	Color closestSphereColor = closestSphere.color;
	float computeLighting = ComputeLighting(P, N);
	Color resultingColor = {
		.r = (int)((float)closestSphereColor.r * computeLighting),
		.g = (int)((float)closestSphereColor.g * computeLighting),
		.b = (int)((float)closestSphereColor.b * computeLighting)
	};

	return resultingColor;
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

	float t1 = (-b + sqrtf(discriminant)) / (2 * a);
	float t2 = (-b - sqrtf(discriminant)) / (2 * a);

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

float ComputeLighting(Vector3D point, Vector3D normal)
{
	float intensity = 0.0f;

	for (int i = 0; i < LIGHTS; i++)
	{
		Light light = SCENE_L[i];

		if (light.type == AMBIENT_LIGHT)
		{
			intensity += light.intensity;
		}
		else
		{
			Vector3D direction = { 0.0f };

			if (light.type == POINT_LIGHT)
			{
				direction = VectorSubstract(light.coords, point);
			}
			else
			{
				direction = light.coords;
			}

			float n_dot_l = DotProduct(normal, direction);

			if (n_dot_l > 0.0f)
			{
				intensity += light.intensity * n_dot_l / (Length(normal) * Length(direction));
			}
		}
	}

	return intensity;
}
/* */

#endif
