#ifndef MICROFACETBRDF_H
#define MICROFACETBRDF_H

#include "BxDF.h"
#include "Fresnel.h"
#include "glm/vec3.hpp"
#include "MicrofacetDistribution.h"

struct MicrofacetBRDF : BxDF
{
	glm::vec3 R;
	MicrofacetDistribution* distr;
	Fresnel* fresnel;
	
	MicrofacetBRDF(const glm::vec3& R, MicrofacetDistribution* distr, Fresnel* fresnel);
	~MicrofacetBRDF();
	float Pdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal) const;
	glm::vec3 f(const glm::vec3& wo, const glm::vec3& normal, const glm::vec3& wi) const;
	glm::vec3 Samplef(const glm::vec3& wo, const float u[2], const glm::vec3& normal,
							  glm::vec3* wi, float* pdf, BxDFType* sampled_type) const;
};

#endif
