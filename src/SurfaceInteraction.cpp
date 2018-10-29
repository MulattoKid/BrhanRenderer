#include "BSDF.h"
#include "MemoryPool/MemoryPool.h"
#include "Scene.h"
#include "Shape.h"
#include "SurfaceInteraction.h"

void SurfaceInteraction::Delete(MemoryPool* mem_pool, const int thread_id)
{
	if (bsdf != NULL)
	{
		bsdf->Delete(mem_pool, thread_id);
	}
}

void SurfaceInteraction::ComputeScatteringFunctions(MemoryPool* mem_pool, const int thread_id)
{
	shape->ComputeScatteringFunctions(this, mem_pool, thread_id);
}

glm::vec3 SurfaceInteraction::Le(const Scene& scene)
{
	if (shape->IsAreaLight())
	{
		return scene.area_lights[shape->area_light_index]->L(Point(), wo);
	}
	
	return glm::vec3(0.0f);
}
