#include "BSDF.h"
#include "BxDF.h"
#include "DirectLightingIntegrator.h"
#include "glm/geometric.hpp"
#include "IntegratorUtilities.h"

DirectLightingIntegrator::~DirectLightingIntegrator()
{}

glm::vec3 DirectLightingIntegrator::Li(const Scene& scene, Ray* ray, RNG* rngs, MemoryPool* mem_pool, const int thread_id, const unsigned int depth, const unsigned int max_depth) const
{
	RNG rng = rngs[thread_id];
	glm::vec3 L(0.0f);
	
	SurfaceInteraction isect;
	if (!scene.Intersect(ray, &isect, 0.0f, 10000.0f)) { return L; }
	isect.ComputeScatteringFunctions(mem_pool, thread_id);
	
	if (isect.shape->IsAreaLight())
	{
		L += isect.Le(scene);
	}
	L += UniformSampleOne(scene, isect, rngs[thread_id]);
	
	if (depth >= max_depth) { return L; }
	float u[2];
	rng.Uniform2D(u);
	glm::vec3 wi(0.0f);
	float pdf;
	BxDFType sampled_type;
	glm::vec3 f = isect.bsdf->Samplef(rng, isect.wo, u, BxDFType(BSDF_SPECULAR | BSDF_REFLECTION), isect.normal, &wi, &pdf, &sampled_type);
	f *= glm::abs(glm::dot(isect.normal, wi));
	if (pdf != 0.0f && f != glm::vec3(0.0f))
	{
		Ray r = SpawnRayWithOffset(isect.point, wi, isect.normal);
		isect.Delete(mem_pool, thread_id);
		L += (f * Li(scene, &r, rngs, mem_pool, thread_id, depth + 1, max_depth)) / pdf;
	}
	
	return L;
}
