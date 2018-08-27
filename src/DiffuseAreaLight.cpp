#include "DiffuseAreaLight.h"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include "Shape.h"

glm::vec3 DiffuseAreaLight::L(const glm::vec3& point, const glm::vec3& wo) const
{
	const bool same_hemisphere = glm::dot(shape->Normal(point), wo) > 0.0f;
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
glm::vec3 DiffuseAreaLight::SampleLi(const SurfaceInteraction& isect, const float u[2], glm::vec3* sample_point, glm::vec3* wi, float* pdf) const
{
	const glm::vec3 tmp_sample_point = shape->Sample(u); //Intersection point to sample point on light
	const glm::vec3 tmp_wi = glm::normalize(tmp_sample_point - isect.point);
	*sample_point = tmp_sample_point;
	*wi = tmp_wi;
	*pdf = PdfLi(isect, tmp_wi);
	return L(tmp_sample_point, -tmp_wi);
}
