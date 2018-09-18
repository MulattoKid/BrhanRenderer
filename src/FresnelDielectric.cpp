#include "FresnelDielectric.h"

FresnelDielectric::FresnelDielectric(const float eta_i, const float eta_t)
{
	this->eta_i = eta_i;
	this->eta_t = eta_t;
}

FresnelDielectric::~FresnelDielectric()
{}

glm::vec3 FresnelDielectric::Evaluate(const glm::vec3& wi, const glm::vec3& normal) const
{
	return Dielectric(wi, normal, eta_i, eta_t);
}
