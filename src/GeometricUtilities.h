#ifndef GEOMETRICUTILITIES_H
#define GEOMETRICUTILITIES_H

#include "glm/mat3x3.hpp"
#include "glm/vec3.hpp"

glm::vec3 PickVectorPerpendicularTo(const glm::vec3& v);
glm::mat3 RotationToAlignAToB(const glm::vec3& a, const glm::vec3& b);
glm::vec3 ProjectVectorOntoPlane(const glm::vec3& v, const glm::vec3& plane_normal);
bool SameHemisphere(const glm::vec3& a, const glm::vec3& b, const glm::vec3& normal);
float UniformHemispherePdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal);
glm::vec3 UniformSampleHemisphere(const float u[2], const glm::vec3& normal);
float CosineHemispherePdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal);
glm::vec3 CosineSampleHemisphere(const float u[2], const glm::vec3& normal);

#endif
