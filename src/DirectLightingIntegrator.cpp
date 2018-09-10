#include "DirectLightingIntegrator.h"
#include "IntegratorUtilities.h"

glm::vec3 DirectLightingIntegrator::Li(const Scene& scene, const SurfaceInteraction& isect, RNG& rng) const
{
	glm::vec3 L(0.0f);
	
	if (isect.shape->IsAreaLight())
	{
		L += scene.area_lights[isect.shape->area_light_index]->L(isect.point, isect.wo);
	}
	L += UniformSampleOne(scene, isect, rng);
	
	return L;
}
