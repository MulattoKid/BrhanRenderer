#ifndef SPHERE_H
#define SPHERE_H

#include "glm/vec3.hpp"
#include "Ray.h"

struct Sphere
{
	glm::vec3 center;
	float radius;
	
	Sphere(const glm::vec3& center, const float radius);
	bool Intersect(Ray* ray) const;
	glm::vec3 Normal(const glm::vec3& point) const;
};

#endif
