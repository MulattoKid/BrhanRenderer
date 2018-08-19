#ifndef RAY_H
#define RAY_H

#include "glm/vec3.hpp"

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;

	Ray(const glm::vec3& origin, const glm::vec3& direction);
	glm::vec3 At(float t) const;
};

#endif