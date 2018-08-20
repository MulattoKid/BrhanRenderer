#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Ray.h"

struct Geometry
{
	virtual bool Intersect(Ray* ray, const float t_min, const float t_max) const = 0;
};

#endif
