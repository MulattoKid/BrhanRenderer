#ifndef PLASTICMATERIAL_H
#define PLASTICMETERIAL_H

#include "Material.h"

struct PlasticMaterial : Material
{
	glm::vec3 Kd;
	glm::vec3 Ks;
	//TODO

	PlasticMaterial(const glm::vec3& Kd, const glm::vec3& Ks);
	void Info() const;
	void ComputeScatteringFunctions(SurfaceInteraction* isect, MemoryPool* mem_pool, const int thread_id) const;
};

#endif
