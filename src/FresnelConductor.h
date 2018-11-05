#ifndef FRESNELCONDUCTOR_H
#define FRESNELCONDUCTOR_H

#include "Fresnel.h"
#include "glm/vec3.hpp"

struct FresnelConductor : Fresnel
{
	glm::vec3 eta_i;
	glm::vec3 eta_t;
	glm::vec3 k_t; //Absorbtion coefficient

	FresnelConductor(const glm::vec3& eta_i, const glm::vec3& eta_t, const glm::vec3& k_t);
	~FresnelConductor();
	glm::vec3 Evaluate(const glm::vec3& wi, const glm::vec3& normal) const;
};

#endif
