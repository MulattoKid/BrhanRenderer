#include "LambertianBRDF.h"

#include "glm/gtc/constants.hpp"

LambertianBRDF::LambertianBRDF(const glm::vec3& R) : BxDF(type)
{
	this->R = R;
}

glm::vec3 LambertianBRDF::f(const glm::vec3& wo, const glm::vec3& wi) const
{
	return R / glm::one_over_pi<float>();
}
