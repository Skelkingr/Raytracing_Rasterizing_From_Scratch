#ifndef UTIL_H
#define UTIL_H

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
	double specular;
	double reflective;
} Sphere;

typedef struct ClosestIntersect
{
	Sphere sphere;
	double closest_t;
} ClosestIntersect;

#define BACKGROUND_COLOR (Color){ 0, 0, 0 }
#define CANVAS_WIDTH 600
#define CANVAS_HEIGHT 600
#define ESPILON 0.001
#define INF (1 << 30)
#define PROJECTION_PLANE_D 1
#define RECURSION_LIMIT 3
#define SPHERES 4
#define VIEWPORT_WIDTH 1
#define VIEWPORT_HEIGHT 1

static const Sphere SCENE_S[SPHERES] = {
	{.center = {.x = 0.0, .y = -1.0, .z = 3.0}, .radius = 1, .color = {.r = 255, .g = 0, .b = 0}, .specular = 500.0, .reflective = 0.2},
	{.center = {.x = 2.0, .y = 0.0, .z = 4.0}, .radius = 1, .color = {.r = 0, .g = 0, .b = 255}, .specular = 500.0, .reflective = 0.3},
	{.center = {.x = -2.0, .y = 0.0, .z = 4.0}, .radius = 1, .color = {.r = 0, .g = 255, .b = 0}, .specular = 10.0, .reflective = 0.4},
	{.center = {.x = 0.0, .y = -5001.0, .z = 0.0}, .radius = 5000, .color = {.r = 255, .g = 255, .b = 0}, .specular = 1000.0, .reflective = 0.5}
};

static const Light SCENE_L[LIGHTS] = {
	{.type = AMBIENT_LIGHT, .intensity = 0.2, .coords = {.x = 0.0, .y = 0.0, .z = 0.0}},
	{.type = POINT_LIGHT, .intensity = 0.6, .coords = {.x = 2.0, .y = 1.0, .z = 0.0}},
	{.type = DIRECTIONAL_LIGHT, .intensity = 0.2, .coords = {.x = 1.0, .y = 4.0, .z = 4.0}}
};

/* Functions prototypes */
void PutPixel(int x, int y, Color color);
void Render();
Vector3D CanvasToViewport(int x, int y);
ClosestIntersect ClosestIntersection(Vector3D O, Vector3D D, double t_min, double t_max);
Vector3D ReflectRay(Vector3D R, Vector3D N);
Color TraceRay(Vector3D O, Vector3D D, double t_min, double t_max, int recursionDepth);
Color ClampColor(Color color);
Array2D IntersectRaySphere(Vector3D O, Vector3D D, Sphere sphere);
bool SphereIsNull(Sphere sphere);
double ComputeLighting(Vector3D P, Vector3D N, Vector3D V, double s);
/* */

/* Actual functions */
void PutPixel(int x, int y, Color color)
{
	x = (canvas.width / 2) + x;
	y = (canvas.height / 2) - y - 1;

	if (x < 0 || x >= canvas.width || y < 0 || y >= canvas.height)
	{
		return;
	}

	canvas.pixels[(y * canvas.width) + x] = RGB(color.b, color.g, color.r);
}

void Render()
{
	Vector3D O = { 0.0, 0.0, 0.0 };

	for (int x = -canvas.width / 2; x < canvas.width / 2; x++)
	{
		for (int y = -canvas.height / 2; y < canvas.height / 2; y++)
		{
			Vector3D D = CanvasToViewport(x, y);
			Color color = TraceRay(O, D, 1, INF, RECURSION_LIMIT);
			PutPixel(x, y, ClampColor(color));
		}
	}
}

Vector3D CanvasToViewport(int x, int y)
{
	return (Vector3D) {
		x * ((double)VIEWPORT_WIDTH / CANVAS_WIDTH),
		y * ((double)VIEWPORT_HEIGHT / CANVAS_HEIGHT),
		PROJECTION_PLANE_D
	};
}

ClosestIntersect ClosestIntersection(Vector3D O, Vector3D D, double t_min, double t_max)
{
	double closest_t = INF;
	Sphere closestSphere = { 0 };

	for (int i = 0; i < SPHERES; i++)
	{
		Sphere sphere = SCENE_S[i];

		Array2D t = IntersectRaySphere(O, D, sphere);

		if ((t.a <= t_max && t.a >= t_min) && t.a < closest_t)
		{
			closest_t = t.a;
			closestSphere = sphere;
		}

		if ((t.b <= t_max && t.b >= t_min) && t.b < closest_t)
		{
			closest_t = t.b;
			closestSphere = sphere;
		}
	}

	return (ClosestIntersect) { closestSphere, closest_t };
}

Vector3D ReflectRay(Vector3D R, Vector3D N)
{
	return VectorSubstract(ScalarMul(2.0 * DotProduct(R, N), N), R);
}

Color TraceRay(Vector3D O, Vector3D D, double t_min, double t_max, int recursionDepth)
{
	ClosestIntersect closestIntersection = ClosestIntersection(O, D, t_min, t_max);

	Sphere closestSphere = closestIntersection.sphere;
	double closest_t = closestIntersection.closest_t;

	if (SphereIsNull(closestSphere))
	{
		return BACKGROUND_COLOR;
	}

	Vector3D P = VectorAdd(O, ScalarMul(closest_t, D));
	Vector3D N = VectorSubstract(P, closestSphere.center);
	N = ScalarMul(1.0 / Length(N), N);

	Color closestSphereColor = closestSphere.color;
	double lighting = ComputeLighting(P, N, ScalarMul(-1.0, D), closestSphere.specular);
	Color localColor = {
		.r = (int)((double)closestSphereColor.r * lighting),
		.g = (int)((double)closestSphereColor.g * lighting),
		.b = (int)((double)closestSphereColor.b * lighting)
	};

	double r = closestSphere.reflective;

	if (recursionDepth <= 0 || r <= 0.0f)
	{
		return localColor;
	}

	Vector3D R = ReflectRay(ScalarMul(-1.0, D), N );

	Color reflectedColor = TraceRay(P, R, ESPILON, INF, recursionDepth - 1);

	return (Color) {
		.r = (int)((double)(localColor.r) * (1 - r) + (double)(reflectedColor.r) * r),
		.g = (int)((double)(localColor.g) * (1 - r) + (double)(reflectedColor.g) * r),
		.b = (int)((double)(localColor.b) * (1 - r) + (double)(reflectedColor.b) * r)
	};
}

Color ClampColor(Color color)
{
	return (Color) {
		.r = min(255, max(0, color.r)),
		.g = min(255, max(0, color.g)),
		.b = min(255, max(0, color.b))
	};
}

Array2D IntersectRaySphere(Vector3D O, Vector3D D, Sphere sphere)
{
	int r = sphere.radius;
	Vector3D CO = VectorSubstract(O, sphere.center);

	double a = DotProduct(D, D);
	double b = 2 * DotProduct(CO, D);
	double c = DotProduct(CO, CO) - r * r;

	double discriminant = b * b - 4 * a * c;

	if (discriminant < 0)
	{
		return (Array2D) { INF, INF };
	}

	double t1 = (-b + sqrt(discriminant)) / (2 * a);
	double t2 = (-b - sqrt(discriminant)) / (2 * a);

	return (Array2D) { t1, t2 };
}

bool SphereIsNull(Sphere sphere)
{
	bool centerIsNull = sphere.center.x == 0.0f && sphere.center.y == 0.0f && sphere.center.z == 0.0f;
	bool colorIsNull = sphere.color.r == 0 && sphere.color.g == 0 && sphere.color.b == 0;

	if (centerIsNull && sphere.radius == 0 && colorIsNull)
		return true;

	return false;
}

double ComputeLighting(Vector3D P, Vector3D N, Vector3D V, double s)
{
	double intensity = 0.0f;
	double t_max;

	for (int i = 0; i < LIGHTS; i++)
	{
		Light light = SCENE_L[i];

		if (light.type == AMBIENT_LIGHT)
		{
			intensity += light.intensity;
		}
		else
		{
			Vector3D L = { 0.0f };
			Vector3D R = { 0.0f };

			if (light.type == POINT_LIGHT)
			{
				L = VectorSubstract(light.coords, P);
				t_max = 1.0f;
			}
			else
			{
				L = light.coords;
				t_max = INF;
			}

			// Shadow check
			ClosestIntersect shadow = ClosestIntersection(P, L, ESPILON, t_max);

			if (!SphereIsNull(shadow.sphere))
			{
				continue;
			}
				
			// Diffuse
			double n_dot_l = DotProduct(N, L);

			if (n_dot_l > 0.0f)
			{
				intensity += light.intensity * n_dot_l / (Length(N) * Length(L));
			}

			// Specular
			if (s != -1)
			{
				R = ReflectRay(L, N);

				double r_dot_v = DotProduct(R, V);

				if (r_dot_v > 0.0f)
				{
					intensity += light.intensity * pow(r_dot_v / (Length(R) * Length(V)), s);
				}
			}
		}
	}

	return intensity;
}
/* */

#endif
