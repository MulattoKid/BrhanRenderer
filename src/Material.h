#ifndef MATERIAL_H
#define MATERIAL_H

#include "MemoryPool.h"
#include "SurfaceInteraction.h"

struct Material
{
	virtual void Info() const = 0;
	virtual void ComputeScatteringFunctions(SurfaceInteraction* isect, MemoryPool* mem_pool, const int thread_id) const = 0;
};

#endif
