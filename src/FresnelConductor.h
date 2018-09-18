#ifndef FRESNELCONDUCTOR_H
#define FRESNELCONDUCTOR_H

#include "Fresnel.h"

struct FresnelConductor : Fresnel
{
	FresnelConductor();
	glm::vec3 Evaluate(const glm::vec3& wi, const glm::vec3& normal) const;
};

#endif
