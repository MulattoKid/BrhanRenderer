#ifndef MIRRORMATERIAL_H
#define MIRRORMATERIAL_H

#include "Material.h"
#include "Texture.h"

struct MirrorMaterial : Material
{
	glm::vec3 Ks;
	Texture* t_Ks;

	MirrorMaterial(const glm::vec3& Ks);
	MirrorMaterial(const std::string& map_Ks);
	void Info() const;
	void ComputeScatteringFunctions(SurfaceInteraction* isect) const;
};

#endif
