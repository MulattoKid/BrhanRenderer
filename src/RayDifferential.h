#ifndef RAYDIFFERENTIAL_H
#define RAYDIFFERENTIAL_H

#include "Ray.h"

struct RayDifferential
{
	Ray primary_ray;
	Ray horizontal_offset_ray;
	Ray vertical_offset_ray;
};

#endif
