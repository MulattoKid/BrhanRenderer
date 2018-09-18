#ifndef FRESNEL_H
#define FRESNEL_H

#include "glm/vec3.hpp"

struct Fresnel
{
	virtual ~Fresnel();
	virtual glm::vec3 Evaluate(const glm::vec3& wi, const glm::vec3& normal) const = 0;
};

glm::vec3 Dielectric(const glm::vec3& wi, const glm::vec3& normal, float eta_i, float eta_t);

#endif
