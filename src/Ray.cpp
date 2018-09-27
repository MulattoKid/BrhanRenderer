#include "Ray.h"

#include "glm/geometric.hpp"

Ray::Ray(const glm::vec3& origin, const glm::vec3& dir)
{
	this->origin = origin;
	this->dir = glm::normalize(dir);
	this->t = std::numeric_limits<float>::max();
}

Ray::Ray(const glm::vec3& origin, const glm::vec3& dir, const float delta)
{
	this->origin = origin + (delta * dir);
	this->dir = glm::normalize(dir);
	this->t = std::numeric_limits<float>::max();
}

glm::vec3 Ray::At() const
{
	return origin + (t * glm::normalize(dir));
}

glm::vec3 Ray::At(float t) const
{
	return origin + (t * glm::normalize(dir));
}

Ray SpawnRayWithOffset(const glm::vec3& origin, const glm::vec3 dir, const glm::vec3& normal)
{
	glm::vec3 normal_wi_side = normal;
	//Ray direction is on the opposite side of the normal for the surface it's spawned from
	if (glm::dot(dir, normal) < 0.0f)
	{
		normal_wi_side = -normal_wi_side;
	}
	
	glm::vec3 offset_origin(0.0f);
	for (int i = 0; i < 3; i++)
	{
		if (normal_wi_side[i] > 0.0f)
		{
			offset_origin[i] = std::nextafterf(origin[i], std::numeric_limits<float>::infinity());
		}
		else if (normal_wi_side[i] < 0.0f)
		{
			offset_origin[i] = std::nextafterf(origin[i], -std::numeric_limits<float>::infinity());
		}
	}
	
	return Ray(offset_origin, dir);
}
