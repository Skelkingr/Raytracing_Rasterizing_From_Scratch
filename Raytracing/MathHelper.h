#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <math.h>

typedef struct Array2D
{
	double a;
	double b;
} Array2D;

typedef struct Vector3D
{
	double x;
	double y;
	double z;
} Vector3D;

double DotProduct(Vector3D u, Vector3D v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

Vector3D VectorAdd(Vector3D u, Vector3D v)
{
	return (Vector3D) { .x = u.x + v.x, .y = u.y + v.y, .z = u.z + v.z };
}

Vector3D VectorSubstract(Vector3D v, Vector3D u)
{
	return (Vector3D) { .x = v.x - u.x, .y = v.y - u.y, .z = v.z - u.z };
}

double Length(Vector3D u)
{
	return sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}

Vector3D ScalarMul(float scalar, Vector3D u)
{
	return (Vector3D) { .x = scalar * u.x, .y = scalar * u.y, .z = scalar * u.z };
}

#endif 
