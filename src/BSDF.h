#ifndef BSDF_H
#define BSDF_H

#include "BxDF.h"

struct BSDF
{
	int num_bxdfs = 0;
	static const int max_bxdfs = 8;
	BxDF* bxdfs[max_bxdfs];
	
	BSDF();
	void Add(BxDF* bxdf);
	int NumMatchingComponents(const BxDFType type) const;
	glm::vec3 f(const glm::vec3& wo, const glm::vec3& wi, BxDFType flags) const;
	glm::vec3 Samplef(const glm::vec3& wo, float u[2], const BxDFType type, const glm::vec3& normal,
					  glm::vec3* wi, float* pdf, BxDFType* sampled_type) const;
};

#endif
