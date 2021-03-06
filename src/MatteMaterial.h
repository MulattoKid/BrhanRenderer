#ifndef MATTEMATERIAL_H
#define MATTEMATERIAL_H

#include "Material.h"
#include "Texture.h"

struct MatteMaterial : Material
{
	glm::vec3 Kd;
	Texture* t_Kd;

	MatteMaterial(const glm::vec3& Kd);
	MatteMaterial(const std::string& map_Kd);
	void Info() const;
	void ComputeScatteringFunctions(SurfaceInteraction* isect) const;
};

#endif
