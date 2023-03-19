#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include "MathHelper.h"

#define LIGHTS 3

typedef enum LightSourceType
{
	AMBIENT,
	POINT,
	DIRECTIONAL
} LightSourceType;

typedef struct Light
{
	LightSourceType type;
	float intensity;
	Vector3D location;
} Light;

#endif
