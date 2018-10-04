#include "glm/geometric.hpp"
#include "Ray.h"

Ray::Ray()
{
	this->origin = glm::vec3(0.0f);
	this->dir = glm::vec3(0.0f);
}

Ray::Ray(const glm::vec3& origin, const glm::vec3& dir)
{
	this->origin = origin;
	this->dir = glm::normalize(dir);
	this->t = EFloat(std::numeric_limits<float>::max());
}

Vec3 Ray::AtError() const
{
	return Vec3(origin) + (t * Normalize(Vec3(dir)));
}

Ray SpawnRayWithOffsetNoFlip(const Vec3& origin, const glm::vec3 dir, const glm::vec3& normal)
{	
	glm::vec3 error(origin.x.err, origin.y.err, origin.z.err);
	float d = glm::dot(glm::abs(normal), glm::abs(error));
	glm::vec3 offset = d * normal;
	
	return Ray(glm::vec3(origin.x.f, origin.y.f, origin.z.f) + offset, dir);
}

Ray SpawnRayWithOffset(const Vec3& origin, const glm::vec3 dir, const glm::vec3& normal)
{
	glm::vec3 normal_wi_side = normal;
	//Ray direction is on the opposite side of the normal for the surface it's spawned from
	if (glm::dot(dir, normal) < 0.0f)
	{
		normal_wi_side = -normal_wi_side;
	}
	
	glm::vec3 error(origin.x.err, origin.y.err, origin.z.err);
	float d = glm::dot(glm::abs(normal), glm::abs(error));
	glm::vec3 offset = d * normal_wi_side;
	
	return Ray(glm::vec3(origin.x.f, origin.y.f, origin.z.f) + offset, dir);
}
