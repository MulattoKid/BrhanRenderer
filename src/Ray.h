#ifndef RAY_H
#define RAY_H

#include "glm/vec3.hpp"
#include "Math.h"

struct Ray
{
	glm::vec3 origin;
	glm::vec3 dir;
	EFloat t;
	
	Ray();
	Ray(const glm::vec3& origin, const glm::vec3& dir);
	Vec3 AtError() const;
};

Ray SpawnRayWithOffsetNoFlip(const Vec3& origin, const glm::vec3 dir, const glm::vec3& normal);
Ray SpawnRayWithOffset(const Vec3& origin, const glm::vec3 dir, const glm::vec3& normal);

#endif
