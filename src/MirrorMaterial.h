#ifndef MIRRORMATERIAL_H
#define MIRRORMATERIAL_H

#include "Material.h"

struct MirrorMaterial : Material
{
	glm::vec3 Ks;

	MirrorMaterial(const glm::vec3& Ks);
	void Info() const;
	void ComputeScatteringFunctions(SurfaceInteraction* isect) const;
};

#endif
