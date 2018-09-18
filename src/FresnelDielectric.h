#ifndef FRESNELDIELECTRIC_H
#define FRESNELDIELECTRIC_H

#include "Fresnel.h"

struct FresnelDielectric : Fresnel
{
	//Indices of refraction
	float eta_i;
	float eta_t;

	FresnelDielectric(const float eta_i, const float eta_t);
	glm::vec3 Evaluate(const glm::vec3& wi, const glm::vec3& normal) const;
};

#endif
