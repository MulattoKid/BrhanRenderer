#include "DiffuseAreaLight.h"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"

glm::vec3 DiffuseAreaLight::L(const glm::vec3& point, const glm::vec3& wo) const
{
	bool same_hemisphere = glm::dot(shape->Normal(point), wo) > 0.0f;
	if (!same_hemisphere)
	{
		return glm::vec3(0.0f);
	}
	
	return Lemit;
}

glm::vec3 DiffuseAreaLight::Power() const
{
	return Lemit * shape->Area() * glm::pi<float>();
}
