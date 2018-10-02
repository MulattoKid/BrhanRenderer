#include "Ray.h"

#include "glm/geometric.hpp"

Ray::Ray(const glm::vec3& origin, const glm::vec3& dir)
{
	this->origin = origin;
	this->dir = glm::normalize(dir);
	this->t = EFloat(std::numeric_limits<float>::max());
}

glm::vec3 Ray::At() const
{
	return origin + (t.f * glm::normalize(dir));
}

Ray SpawnRayWithOffsetNoFlip(const glm::vec3& origin, const glm::vec3 dir, const glm::vec3& normal)
{	
	return Ray(origin + (normal * 0.0001f), dir);
}

Ray SpawnRayWithOffset(const glm::vec3& origin, const glm::vec3 dir, const glm::vec3& normal, const EFloat t)
{
	glm::vec3 normal_wi_side = normal;
	//Ray direction is on the opposite side of the normal for the surface it's spawned from
	if (glm::dot(dir, normal) < 0.0f)
	{
		normal_wi_side = -normal_wi_side;
	}
	
	Vec3 ray_at(origin);
	normal_wi_side *= 0.0001f;
	
	return Ray(origin + normal_wi_side, dir);
}
