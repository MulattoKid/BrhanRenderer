#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "glm/vec3.hpp"
#include "RNG.h"
#include "Scene.h"
#include "SurfaceInteraction.h"

enum IntegratorType
{
	DIRECT_LIGHTING_INTEGRATOR
};

struct Integrator
{
	virtual ~Integrator();
	virtual glm::vec3 Li(const Scene& scene, Ray* ray, RNG& rng, const unsigned int depth, const unsigned int max_depth) const = 0;
};

#endif
