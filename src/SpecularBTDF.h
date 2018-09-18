#ifndef SPECULARBTDF_H
#define SPECULARBTDF_H

#include "BxDF.h"
#include "FresnelDielectric.h"

struct SpecularBTDF : BxDF
{
	glm::vec3 T;
	float eta_outside;
	float eta_inside;
	FresnelDielectric fresnel_dielectric;

	SpecularBTDF(const glm::vec3& T, const float eta_i, const float eta_t);
	~SpecularBTDF();
	float Pdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal) const;
	glm::vec3 f(const glm::vec3& wo, const glm::vec3& wi) const;
	glm::vec3 Samplef(const glm::vec3& wo, const float u[2], const glm::vec3& normal,
					  glm::vec3* wi, float* pdf, BxDFType* sampled_type) const;
};

#endif
