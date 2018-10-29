#ifndef PATHINTEGRATOR_H
#define PATHINTEGRATOR_H

#include "Integrator.h"

struct PathIntegrator : Integrator
{
	~PathIntegrator();
	glm::vec3 Li(const Scene& scene, RayDifferential* ray, RNG* rngs, MemoryPool* mem_pool, const int thread_id, const unsigned int depth, const unsigned int max_depth) const;
};

#endif
