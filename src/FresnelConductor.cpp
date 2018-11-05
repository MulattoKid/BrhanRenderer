#include "FresnelConductor.h"

FresnelConductor::FresnelConductor(const glm::vec3& eta_i, const glm::vec3& eta_t, const glm::vec3& k_t)
{
	this->eta_i = eta_i;
	this->eta_t = eta_t;
	this->k_t = k_t;
}

FresnelConductor::~FresnelConductor()
{}

glm::vec3 FresnelConductor::Evaluate(const glm::vec3& wo, const glm::vec3& normal) const
{
	return Conductor(wo, normal, eta_i, eta_t, k_t);
}
