#ifndef MATTEMATERIAL_H
#define MATTEMATERIAL_H

#include "Material.h"

struct MatteMaterial : Material
{
	glm::vec3 Kd;

	MatteMaterial(const glm::vec3& Kd);
	void Info() const;
	void ComputeScatteringFunctions(SurfaceInteraction* isect) const;
};

#endif
