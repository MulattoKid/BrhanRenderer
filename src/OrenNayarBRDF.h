#ifndef ORENNAYARBRDF_H
#define ORENNAYARBRDF_H

#include "BxDF.h"

struct OrenNayarBRDF : BxDF
{
	glm::vec3 R;
	float A;
	float B;

	OrenNayarBRDF(const glm::vec3& R, const float sigma);
	~OrenNayarBRDF();
	virtual glm::vec3 f(const glm::vec3& wo, const glm::vec3& normal, const glm::vec3& wi) const;
};

#endif
