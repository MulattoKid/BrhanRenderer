#ifndef FRESNELNOOP_H
#define FRESNELNOOP_H

#include "Fresnel.h"

struct FresnelNoOp : Fresnel
{
	glm::vec3 Evaluate(const glm::vec3& wi, const glm::vec3& normal) const;
};

#endif
