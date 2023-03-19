#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <math.h>

typedef struct Array2D
{
	float a;
	float b;
} Array2D;

typedef struct Vector3D
{
	float x;
	float y;
	float z;
} Vector3D;

float DotProduct(Vector3D u, Vector3D v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

Vector3D VectorAdd(Vector3D u, Vector3D v)
{
	Vector3D result = { .x = u.x + v.x, .y = u.y + v.y, .z = u.z + v.z };

	return result;
}

Vector3D VectorSubstract(Vector3D v, Vector3D u)
{
	Vector3D result = { .x = v.x - u.x, .y = v.y - u.y, .z = v.z - u.z };

	return result;
}

float Length(Vector3D u)
{
	return sqrtf(u.x * u.x + u.y * u.y + u.z * u.z);
}

Vector3D ScalarMul(float scalar, Vector3D u)
{
	Vector3D result = { .x = scalar * u.x, .y = scalar * u.y, .z = scalar * u.z };

	return result;
}

#endif 
