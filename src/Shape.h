#ifndef SHAPE_H
#define SHAPE_H

#include "glm/vec3.hpp"
#include "Ray.h"

struct Shape
{
	virtual bool Intersect(Ray* ray, const float t_min, const float t_max) const = 0;
};

#endif
