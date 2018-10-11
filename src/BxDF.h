#ifndef BRDF_H
#define BRDF_H

#include "glm/vec3.hpp"

enum BxDFType
{
	BSDF_REFLECTION   = 1 << 0,
	BSDF_TRANSMISSION = 1 << 1,
	BSDF_DIFFUSE      = 1 << 2,
	BSDF_GLOSSY       = 1 << 3,
	BSDF_SPECULAR     = 1 << 4,
	BSDF_ALL          = BSDF_REFLECTION | BSDF_TRANSMISSION | 
						BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR
};

struct BxDF
{
	BxDFType type;
	
	BxDF(BxDFType type);
	virtual ~BxDF();
	bool MatchesFlags(BxDFType t) const;
	virtual float Pdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal) const;
	virtual glm::vec3 f(const glm::vec3& wo, const glm::vec3& normal, const glm::vec3& wi) const = 0;
	virtual glm::vec3 Samplef(const glm::vec3& wo, const float u[2], const glm::vec3& normal,
							  glm::vec3* wi, float* pdf, BxDFType* sampled_type) const;
};

#endif
