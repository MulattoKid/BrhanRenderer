#include "glm/geometric.hpp"
#include "SpecularBTDF.h"

#include "glm/trigonometric.hpp"

SpecularBTDF::SpecularBTDF(const glm::vec3& T, const float eta_outside, const float eta_inside) :
														BxDF(BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR), SPECULAR_BTDF),
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

glm::vec3 SpecularBTDF::f(const glm::vec3& wo, const glm::vec3& normal, const glm::vec3& wi) const
{
	return glm::vec3(0.0f);
}

glm::vec3 SpecularBTDF::Samplef(const glm::vec3& wo, const float u[2], const glm::vec3& normal,
					            glm::vec3* wi, float* pdf, BxDFType* sampled_type) const
{
	const bool exiting = glm::dot(wo, normal) <= 0.0f;
	const float eta_from = exiting ? eta_inside : eta_outside;
	const float eta_to = exiting ? eta_outside : eta_inside;
	const glm::vec3 normal_wo_side = exiting ? -normal : normal;
	
	*wi = glm::refract(-wo, normal_wo_side, eta_from / eta_to); //GLM expects I to be incident -> flip wo
	*pdf = 1.0f;
	*sampled_type = type;
	
	const glm::vec3 Fr = fresnel_dielectric.Evaluate(*wi, normal);
	glm::vec3 Ft = glm::vec3(1.0f) - Fr;
	//Account for non-symmetry with transmission to different medium p.961
	Ft *= glm::pow(eta_from, 2) / glm::pow(eta_to, 2);
	return T * Ft / glm::abs(glm::dot(*wi, -normal_wo_side));
}
