#ifndef RAY_H
#define RAY_H

#include "glm/vec3.hpp"

struct Ray
{
	glm::vec3 origin;
	glm::vec3 dir;
	float t;

	Ray(const glm::vec3& origin, const glm::vec3& dir);
	Ray(const glm::vec3& origin, const glm::vec3& dir, const float delta);
	glm::vec3 At() const;
	glm::vec3 At(float t) const;
};

Ray SpawnRayWithOffsetNoFlip(const glm::vec3& origin, const glm::vec3 dir, const glm::vec3& normal);
Ray SpawnRayWithOffset(const glm::vec3& origin, const glm::vec3 dir, const glm::vec3& normal);

#endif
