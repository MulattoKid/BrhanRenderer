#ifndef PATHINTEGRATOR_H
#define PATHINTEGRATOR_H

#include "Integrator.h"

struct PathIntegrator : Integrator
{
	~PathIntegrator();
	glm::vec3 Li(const Scene& scene, Ray* ray, RNG& rng, const unsigned int depth, const unsigned int max_depth) const;
};

#endif
