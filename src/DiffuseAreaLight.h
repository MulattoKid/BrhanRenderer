#ifndef DIFFUSEAREALIGHT_H
#define DIFFUSEAREALIGHT_H

#include "AreaLight.h"

struct DiffuseAreaLight : AreaLight
{
	glm::vec3 Lemit; //Emitted radiance

	
	glm::vec3 L(const glm::vec3& point, const glm::vec3& wo) const;
	glm::vec3 Power() const;
};

#endif
