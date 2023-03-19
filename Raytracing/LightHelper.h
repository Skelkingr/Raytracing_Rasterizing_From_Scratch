#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include "MathHelper.h"

#define LIGHTS 3

typedef enum LightSourceType
{
	AMBIENT_LIGHT,
	POINT_LIGHT,
	DIRECTIONAL_LIGHT
} LightSourceType;

typedef struct Light
{
	LightSourceType type;
	float intensity;
	Vector3D coords;
} Light;

#endif
