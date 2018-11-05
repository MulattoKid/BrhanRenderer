#ifndef FRESNEL_H
#define FRESNEL_H

#include "glm/vec3.hpp"

struct Fresnel
{
	virtual ~Fresnel();
	virtual glm::vec3 Evaluate(const glm::vec3& wo, const glm::vec3& normal) const = 0;
};

glm::vec3 Conductor(const glm::vec3& wo, const glm::vec3& normal, const glm::vec3& eta_from, const glm::vec3& eta_to, const glm::vec3& k_to);
glm::vec3 Dielectric(const glm::vec3& wo, const glm::vec3& normal, float eta_from, float eta_to);

#endif
