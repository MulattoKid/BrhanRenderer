#ifndef QUAD_H
#define QUAD_H

#include "Shape.h"

struct Quad : Shape
{
	glm::vec3 v[4];
	glm::vec3 n[4];
	glm::vec2 uv[4];

	Quad();
	glm::vec3 Normal(const glm::vec3& point) const;
	glm::vec2 UV(const glm::vec3& point) const;
	float Area() const;
	glm::vec3 Sample(RNG& rng, const float u[2]) const;
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const;
	void ComputeDifferentialSurface(glm::vec3* dpdu, glm::vec3* dpdv) const;
};

#endif
