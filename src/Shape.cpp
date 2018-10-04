#include <cfloat>
#include "glm/geometric.hpp"
#include "Logger.h"
#include "Shape.h"

Shape::Shape(const bool double_sided)
{
	this->double_sided = double_sided;
}

//p.838
float Shape::Pdf(const SurfaceInteraction& isect, const glm::vec3& wi) const
{
	Ray ray = SpawnRayWithOffsetNoFlip(isect.point, wi, isect.normal);
	SurfaceInteraction isect_light;
	if (!Intersect(&ray, &isect_light, 0.0f, 10000.0f)) { return 0.0f; }
	isect_light.point = ray.AtError();
	isect_light.normal = isect_light.shape->Normal(isect_light.Point());
	isect_light.wo = -ray.dir;
	
	float pdf = 1.0f / Area();
	pdf *= glm::pow(glm::length(ray.origin - isect_light.Point()), 2) / glm::abs(glm::dot(isect_light.normal, isect_light.wo));
	
	return pdf;
}

void Shape::ComputeScatteringFunctions(SurfaceInteraction* isect) const
{
	material->ComputeScatteringFunctions(isect);
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
	return area_light_index == -1 ? false : true;
}
