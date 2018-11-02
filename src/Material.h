#ifndef MATERIAL_H
#define MATERIAL_H

#include "MemoryPool/MemoryPool.h"
#include "SurfaceInteraction.h"

enum MemoryPoolObjectTypes
{
	MEM_POOL_BSDF = 0,
	MEM_POOL_BxDF = 1,
	MEM_POOL_FRESNEL = 2
};

struct Material
{
	virtual void Info() const = 0;
	virtual void ComputeScatteringFunctions(SurfaceInteraction* isect, MemoryPool* mem_pool, const int thread_id) const = 0;
};

#endif
