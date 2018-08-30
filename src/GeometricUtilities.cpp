#include "GeometricUtilities.h"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"

bool SameHemisphere(const glm::vec3& a, const glm::vec3& b, const glm::vec3& normal)
{
	//Check if they are in the same hemisphere
	if (glm::dot(a, normal) * glm::dot(b, normal) > 0.0f) { return true; }
	return false;
}

glm::vec3 UniformSampleHemisphere(const float u[2], const glm::vec3& normal)
{
	//TODO
	return glm::vec3(0.0f);
}

float UniformHemispherePdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal)
{
	if (SameHemisphere(wo, wi, normal))
	{
		return glm::two_over_pi<float>();
	}
	return 0.0f;
}
