#ifndef FRESNELDIELECTRIC_H
#define FRESNELDIELECTRIC_H

#include "Fresnel.h"

struct FresnelDielectric : Fresnel
{
	FresnelDielectric();
	glm::vec3 Evaluate() const;
};

#endif