#ifndef QUAD_H
#define QUAD_H

#include "Shape.h"
#include "SurfaceInteraction.h"

struct Quad : Shape
{
	glm::vec3 v[4];
	glm::vec3 n[4];
	glm::vec2 uv[4];

	glm::vec3 Normal(const glm::vec3& point) const;
	glm::vec2 UV(const glm::vec3& point) const;
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const;
};

#endif
