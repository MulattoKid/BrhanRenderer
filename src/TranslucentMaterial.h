#ifndef TRANSLUCENTMATERIAL_H
#define TRANSLUCENTMATERIAL_H

#include "glm/vec3.hpp"
#include "Material.h"

struct TranslucentMaterial : Material
{
	glm::vec3 T;
	float eta_outside;
	float eta_inside;

	TranslucentMaterial(const glm::vec3& T, const float eta_outside, const float eta_inside);
	void Info() const;
	void ComputeScatteringFunctions(SurfaceInteraction* isect) const;
};

#endif
