#ifndef INTEGRATORUTILITIES_H
#define INTEGRATORUTILITIES_H

#include "AreaLight.h"
#include "RNG.h"
#include "Scene.h"
#include "SurfaceInteraction.h"

glm::vec3 UniformSampleAll();
glm::vec3 UniformSampleOne(const Scene& scene, const SurfaceInteraction& isect, RNG& rng);
glm::vec3 EstimateDirect(const Scene& scene, RNG& rng, const AreaLight* area_light, const SurfaceInteraction& isect, const float u_light[2], float u_scattering[2]);

#endif
