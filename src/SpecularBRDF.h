#ifndef SPECULARBRDF_H
#define SPECULARBRDF_H

#include "BxDF.h"
#include "Fresnel.h"

struct SpecularBRDF : BxDF
{
	glm::vec3 R;
	Fresnel* fresnel;

	SpecularBRDF(const glm::vec3& R, Fresnel* fresnel);
	~SpecularBRDF();
	float Pdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal) const;
	glm::vec3 f(const glm::vec3& wo, const glm::vec3& wi) const;
	glm::vec3 Samplef(const glm::vec3& wo, const float u[2], const glm::vec3& normal,
					  glm::vec3* wi, float* pdf, BxDFType* sampled_type) const;
};

#endif
