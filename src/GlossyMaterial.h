#ifndef GLOSSYMATERIAL_H
#define GLOSSYMETERIAL_H

#include "Material.h"

struct GlossyMaterial : Material
{
	glm::vec3 Kd;
	glm::vec3 Ks;

	GlossyMaterial(const glm::vec3& Kd, const glm::vec3& Ks);
	void ComputeScatteringFunctions(SurfaceInteraction* isect) const;
};

#endif
