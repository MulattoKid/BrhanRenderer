#ifndef GLASSMATERIAL_H
#define GLASSMATERIAL_H

#include "Material.h"

struct GlassMaterial : Material
{
	glm::vec3 R;
	glm::vec3 T;
	float eta_outside;
	float eta_inside;

	GlassMaterial(const glm::vec3& R, const glm::vec3& T);
	void Info() const;
	void ComputeScatteringFunctions(SurfaceInteraction* isect) const;
};

#endif
