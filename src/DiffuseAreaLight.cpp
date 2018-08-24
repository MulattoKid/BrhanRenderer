#include "DiffuseAreaLight.h"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include "Shape.h"

glm::vec3 DiffuseAreaLight::L(const glm::vec3& point, const glm::vec3& wo) const
{
	bool same_hemisphere = glm::dot(shape->Normal(point), wo) > 0.0f;
	if (!same_hemisphere)
	{
		return glm::vec3(0.0f);
	}
	
	return L_emit;
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
glm::vec3 DiffuseAreaLight::SampleLi(const SurfaceInteraction& isect, const float u[2], float* pdf, glm::vec3* wi) const
{
	const glm::vec3 sample_point = shape->Sample(u);
	*wi = glm::normalize(sample_point - isect.point); //Intersection point to sample point on light
	*pdf = PdfLi(isect, *wi);
	return L(sample_point, -*wi);
}
