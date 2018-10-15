#ifndef SHAPE_H
#define SHAPE_H

#include "BoundingBox.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "Material.h"
#include "MTL.h"
#include "Ray.h"
#include "RNG.h"
#include "SurfaceInteraction.h"

struct Shape
{
	BoundingBox bb;
	Material* material = NULL;
	MTL* mtl = NULL;
	int area_light_index = -1;
	bool double_sided;

	Shape(const bool double_sided);
	virtual glm::vec3 Normal(const glm::vec3& point) const = 0;
	virtual glm::vec2 UV(const glm::vec3& point) const = 0;
	virtual float Area() const = 0;
	virtual float Pdf(const SurfaceInteraction& isect, const glm::vec3& wi) const;
	virtual glm::vec3 Sample(RNG& rng, const float u[2]) const = 0;
	virtual bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const = 0;
	virtual bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_less_than) const = 0;
	void ComputeScatteringFunctions(SurfaceInteraction* isect) const;
	glm::vec3 AmbientColor() const;
	glm::vec3 DiffuseColor() const;
	glm::vec3 SpecularColor() const;
	glm::vec3 EmissionColor() const;
	bool IsAreaLight() const;
};

#endif
