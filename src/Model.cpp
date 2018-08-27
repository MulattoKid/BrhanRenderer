#include "Model.h"

bool Model::Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const
{
	bool intersected = false;
	for (const Shape* shape : shapes)
	{
		intersected |= shape->Intersect(ray, isect, t_min, t_max);
	}

	return intersected;
}

bool Model::Intersect(Ray* ray, SurfaceInteraction* isect, const float t_less_than) const
{
	bool intersected = false;
	for (const Shape* shape : shapes)
	{
		intersected |= shape->Intersect(ray, isect, t_less_than);
	}

	return intersected;
}
