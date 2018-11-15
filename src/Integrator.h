#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "Camera.h"
#include "glm/vec3.hpp"
#include "RNG.h"
#include "Scene.h"
#include "SurfaceInteraction.h"

enum IntegratorType
{
	PATH_INTEGRATOR
};

struct Integrator
{
	virtual ~Integrator();
	virtual glm::vec3 Li(const Scene& scene, RayDifferential* ray, RNG* rngs, const int thread_id, const unsigned int depth, const unsigned int max_depth) const = 0;
};

#endif
