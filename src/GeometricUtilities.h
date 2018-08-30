#ifndef GEOMETRICUTILITIES_H
#define GEOMETRICUTILITIES_H

#include "glm/vec3.hpp"

bool SameHemisphere(const glm::vec3& a, const glm::vec3& b, const glm::vec3& normal);
glm::vec3 UniformSampleHemisphere(const float u[2], const glm::vec3& normal);
float UniformHemispherePdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal);

#endif
