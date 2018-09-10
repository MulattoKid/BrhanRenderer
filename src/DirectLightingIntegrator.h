#ifndef DIRECTLIGHTINGINTEGRATOR_H
#define DIRECTLIGHTINGINTEGRATOR_H

#include "Integrator.h"

struct DirectLightingIntegrator : Integrator
{
	glm::vec3 Li(const Scene& scene, const SurfaceInteraction& isect, RNG& rng) const;
};

#endif
