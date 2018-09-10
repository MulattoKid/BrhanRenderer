#ifndef DIRECTLIGHTINGINTEGRATOR_H
#define DIRECTLIGHTINGINTEGRATOR_H

#include "Integrator.h"

struct DirectLightingIntegrator : Integrator
{
	~DirectLightingIntegrator();
	glm::vec3 Li(const Scene& scene, Ray* ray, RNG& rng, const unsigned int depth, const unsigned int max_depth) const;
};

#endif
