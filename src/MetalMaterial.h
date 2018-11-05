#ifndef METALMATERIAL_H
#define METALMATERIAL_H

#include "Material.h"

enum MetalType
{
	COPPER
};

struct MetalMaterial : Material
{
	glm::vec3 Ks;
	glm::vec3 N; //Index of refraction
	glm::vec3 K; //Absorbtion coefficient
	
	MetalMaterial(const glm::vec3& Ks, const MetalType m_type);
	void Info() const;
	void ComputeScatteringFunctions(SurfaceInteraction* isect, MemoryPool* mem_pool, const int thread_id) const;
};

#endif
