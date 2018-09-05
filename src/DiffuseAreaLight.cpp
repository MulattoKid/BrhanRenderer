#include "DiffuseAreaLight.h"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include "Shape.h"

glm::vec3 DiffuseAreaLight::L(const glm::vec3& point, const glm::vec3& wo) const
{
	const bool same_hemisphere = glm::dot(shape->Normal(point), wo) > 0.0f;
	if (same_hemisphere)
	{
		return L_emit;
	}
	
	return glm::vec3(0.0f);
}

glm::vec3 DiffuseAreaLight::Power() const
{
	return L_emit * shape->Area() * glm::pi<float>();
}

float DiffuseAreaLight::PdfLi(const SurfaceInteraction& isect, const glm::vec3& wi) const
{
	return shape->Pdf(isect, wi);
}

//p.845
glm::vec3 DiffuseAreaLight::SampleLi(const SurfaceInteraction& isect, const float u[2], glm::vec3* sample_point, glm::vec3* wi, float* pdf) const
{
	*sample_point = shape->Sample(u); //Intersection point to sample point on light
	*wi = glm::normalize(*sample_point - isect.point);;
	*pdf = PdfLi(isect, *wi);
	return L(*sample_point, -*wi);
}
