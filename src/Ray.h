#ifndef RAY_H
#define RAY_H

#include "glm/vec3.hpp"
#include "Math.h"

struct Ray
{
	glm::vec3 origin;
	glm::vec3 dir;
	EFloat t;

	Ray(const glm::vec3& origin, const glm::vec3& dir);
	glm::vec3 At() const;
};

Ray SpawnRayWithOffsetNoFlip(const glm::vec3& origin, const glm::vec3 dir, const glm::vec3& normal);
Ray SpawnRayWithOffset(const glm::vec3& origin, const glm::vec3 dir, const glm::vec3& normal, const EFloat t);

#endif
