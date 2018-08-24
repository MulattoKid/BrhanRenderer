#ifndef DIFFUSEAREALIGHT_H
#define DIFFUSEAREALIGHT_H

#include "AreaLight.h"

struct DiffuseAreaLight : AreaLight
{
	glm::vec3 L_emit; //Emitted radiance

	
	glm::vec3 L(const glm::vec3& point, const glm::vec3& wo) const;
	glm::vec3 Power() const;
	float PdfLi(const SurfaceInteraction& isect, const glm::vec3& wi) const;
	glm::vec3 SampleLi(const SurfaceInteraction& isect, const float u[2], float* pdf, glm::vec3* wi) const;
};

#endif
