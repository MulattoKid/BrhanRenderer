#ifndef SPHERE_H
#define SPHERE_H

#include "glm/vec3.hpp"
#include "Shape.h"
#include "SurfaceInteraction.h"

struct Sphere : Shape
{
	glm::vec3 center;
	float radius;
	
	Sphere(const glm::vec3& center, const float radius);
	glm::vec3 Normal(const glm::vec3& point) const;
	glm::vec2 UV(const glm::vec3& point) const;
	float Area() const;
	glm::vec3 Sample(const float u[2]) const;
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const;
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_less_than) const;
};

#endif
