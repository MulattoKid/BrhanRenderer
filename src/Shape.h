#ifndef SHAPE_H
#define SHAPE_H

#include "AreaLight.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "MTL.h"
#include "Ray.h"
#include "SurfaceInteraction.h"

struct Shape
{
	MTL* mtl = NULL;
	AreaLight* area_light = NULL;

	virtual glm::vec3 Normal(const glm::vec3& point) const = 0;
	virtual glm::vec2 UV(const glm::vec3& point) const = 0;
	virtual float Area() const = 0;
	virtual glm::vec3 Sample(const float u[2]) const = 0;
	virtual bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const = 0;
	glm::vec3 AmbientColor() const;
	glm::vec3 DiffuseColor() const;
	glm::vec3 SpecularColor() const;
	glm::vec3 EmissionColor() const;
	bool IsAreaLight() const;
};

#endif
