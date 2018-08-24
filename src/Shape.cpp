#include <cfloat>
#include "glm/geometric.hpp"
#include "Shape.h"

//p.838
float Shape::Pdf(const SurfaceInteraction& isect, const glm::vec3& wi) const
{
	Ray ray(isect.point, wi);
	SurfaceInteraction isect_light;
	if (!Intersect(&ray, &isect_light, 0.0f, FLT_MAX)) { return 0.0f; }
	
	const float distance_squared = glm::pow(glm::length(ray.origin - isect.point), 2);
	return distance_squared / (glm::abs(glm::dot(isect_light.normal, -wi)) * Area());
}

glm::vec3 Shape::AmbientColor() const
{
	if (mtl == NULL)
	{
		return glm::vec3(1.0f);
	}
	
	return mtl->ambient;
}

glm::vec3 Shape::DiffuseColor() const
{
	if (mtl == NULL)
	{
		return glm::vec3(1.0f);
	}
	
	return mtl->diffuse;
}

glm::vec3 Shape::SpecularColor() const
{
	if (mtl == NULL)
	{
		return glm::vec3(1.0f);
	}
	
	return mtl->specular;
}

glm::vec3 Shape::EmissionColor() const
{
	if (mtl == NULL)
	{
		return glm::vec3(1.0f);
	}
	
	return mtl->emission;
}

bool Shape::IsAreaLight() const
{
	return area_light == NULL ? false : true;
}
