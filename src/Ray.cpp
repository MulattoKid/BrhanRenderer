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
