#include "glm/geometric.hpp"
#include "glm/vec3.hpp"
#include "Integrator.h"

Integrator::~Integrator()
{}

float Integrator::Depth(const Scene& scene, Ray* ray) const
{
	SurfaceInteraction isect;
	bool intersected = scene.Intersect(ray, &isect, 0.00001f, 10000.0f);
	if (intersected)
	{
		glm::vec3 isect_point = glm::vec3(isect.point.x.f, isect.point.y.f, isect.point.z.f);
		return glm::length(ray->origin - isect_point);
	}
	else
	{
		return -1.0f;
	}
}
