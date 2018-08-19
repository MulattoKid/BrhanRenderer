#include "Ray.h"

#include "glm/geometric.hpp"

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
{
	this->origin = origin;
	this->direction = direction;
}

glm::vec3 Ray::At(float t) const
{
	return origin + (t * glm::normalize(direction));
}