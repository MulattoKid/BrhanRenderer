#ifndef LAMBERTIANBRDF_H
#define LAMBERTIANBRDF_H

#include "BxDF.h"

struct LambertianBRDF : BxDF
{
	glm::vec3 R;

	LambertianBRDF(const glm::vec3& R);
	~LambertianBRDF();
	virtual glm::vec3 f(const glm::vec3& wo, const glm::vec3& normal, const glm::vec3& wi) const;
};

#endif
