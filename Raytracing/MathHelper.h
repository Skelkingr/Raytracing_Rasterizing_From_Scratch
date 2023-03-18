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

#endif 
