#ifndef MATHHELPER_H
#define MATHHELPER_H

typedef struct Vector3D
{
	float x;
	float y;
	float z;
} Vector3D;

float dotProduct(Vector3D u, Vector3D v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

Vector3D vectorSubstract(Vector3D v, Vector3D u)
{
	Vector3D result = { .x = v.x - u.x, .y = v.y - u.y, .z = v.z - u.z };

	return result;
}

#endif 