#ifndef METALMATERIAL_H
#define METALMATERIAL_H

#include "Material.h"

enum MetalType
{
	ALUMINIUM,
	COPPER,
	GOLD,
	SALT
};

struct MetalMaterial : Material
{
	glm::vec3 Ks;
	glm::vec3 N; //Index of refraction
	glm::vec3 K; //Absorbtion coefficient
	
	MetalMaterial(const glm::vec3& Ks, const MetalType m_type);
	void Info() const;
	void ComputeScatteringFunctions(SurfaceInteraction* isect) const;
};

#endif
