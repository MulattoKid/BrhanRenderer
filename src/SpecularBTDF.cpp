#include "glm/geometric.hpp"
#include "SpecularBTDF.h"

SpecularBTDF::SpecularBTDF(const glm::vec3& T, const float eta_outside, const float eta_inside) : BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR)),
																					   fresnel_dielectric(eta_outside, eta_inside)
{
	this->eta_outside = eta_outside;
	this->eta_inside = eta_inside;
	this->T = T;
}

SpecularBTDF::~SpecularBTDF()
{}

float SpecularBTDF::Pdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal) const
{
	return 0.0f;
}

glm::vec3 SpecularBTDF::f(const glm::vec3& wo, const glm::vec3& wi) const
{
	return glm::vec3(0.0f);
}

glm::vec3 SpecularBTDF::Samplef(const glm::vec3& wo, const float u[2], const glm::vec3& normal,
					            glm::vec3* wi, float* pdf, BxDFType* sampled_type) const
{
	bool exiting = glm::dot(wo, normal) <= 0.0f;
	float eta_i = exiting ? eta_inside : eta_outside;
	float eta_t = exiting ? eta_outside : eta_inside;
	
	*wi = glm::refract(-wo, normal, eta_i / eta_t); //GLM expect I to be incident
	*pdf = 1.0f;
	*sampled_type = type;
	
	glm::vec3 Ft = T * (glm::vec3(1.0f) - fresnel_dielectric.Evaluate(wo, normal));
	return Ft / glm::abs(glm::dot(wo, normal));
}
