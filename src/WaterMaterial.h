#ifndef WATERMATERIAL_H
#define WATERMATERIAL_H

#include "Material.h"

struct WaterMaterial : Material
{
	glm::vec3 R;
	glm::vec3 T;
	float eta_outside;
	float eta_inside;

	WaterMaterial(const glm::vec3& R, const glm::vec3& T);
	void Info() const;
	void ComputeScatteringFunctions(SurfaceInteraction* isect, MemoryPool* mem_pool, const int thread_id) const;
};

#endif
