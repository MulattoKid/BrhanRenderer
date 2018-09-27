#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"

struct Triangle : Shape
{
	glm::vec3 v[3];
	glm::vec3 n[3];
	glm::vec2 uv[3];
	
	Triangle();
	glm::vec3 BarycentricCoefficients(const glm::vec3& point) const;
	glm::vec3 Normal(const glm::vec3& point) const;
	glm::vec2 UV(const glm::vec3& point) const;
	float Area() const;
	glm::vec3 Sample(RNG& rng, const float u[2]) const;
	bool PointIn(const glm::vec3& point) const;
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const;
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_less_than) const;
};

#endif
