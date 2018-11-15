#include "BSDF.h"
#include "BxDF.h"
#include "glm/geometric.hpp"
#include "IntegratorUtilities.h"
#include "Math.h"
#include "PathIntegrator.h"

PathIntegrator::~PathIntegrator()
{}

glm::vec3 PathIntegrator::Li(const Scene& scene, RayDifferential* ray, RNG* rngs, const int thread_id, const unsigned int depth, const unsigned int max_depth) const
{
	RNG& rng = rngs[thread_id];
	glm::vec3 L(0.0f);
	glm::vec3 path_throughput(1.0f);
	bool specular_bounce = false;
	
	for (unsigned int b = 0; b < max_depth; b++)
	{
		SurfaceInteraction isect;
		bool intersected = scene.Intersect(&ray->primary_ray, &isect, 0.00001f, 10000.0f);
		
		//depth==0: this is the first bounce and direct illumination from light
		//			sources haven't been accounted for yet
		//specular_bounce==true: the previous iteration had no way of evaluating
		//			the direct illumination as it's a delta function
		if (b == 0 || specular_bounce)
		{
			if (intersected)
			{
				L += path_throughput * isect.Le(scene);
			}
			//TODO: account for infitie area lights (no geometry)
		}
		
		if (!intersected || b >= max_depth) { break; }
		isect.ComputeDifferentials(*ray, b);
		isect.ComputeScatteringFunctions();
		
		//Sample direct illumination from lights
		glm::vec3 Ld = path_throughput * UniformSampleOne(scene, isect, rng);
		L += Ld;
		
		//Sample BSDF to get new direction
		float u[2];
		rng.Uniform2D(u);
		glm::vec3 wi;
		float pdf;
		BxDFType sampled_type;
		glm::vec3 f = isect.bsdf->Samplef(rng, isect.wo, u, BSDF_ALL, isect.normal, &wi, &pdf, &sampled_type);
		
		if (pdf == 0.0f || f == glm::vec3(0.0f))
		{
			break;
		}
		path_throughput *= f * glm::abs(glm::dot(isect.normal, wi)) / pdf;
		
		if (path_throughput.x <= 0.0f && path_throughput.y <= 0.0f && path_throughput.z <= 0.0f) 
		{
			break;
		}
		specular_bounce = (sampled_type & BSDF_SPECULAR) != 0;
		
		ray->primary_ray = SpawnRayWithOffset(isect.point, wi, isect.normal);
		//isect.Delete(mem_pool, thread_id);
	}
	
	return L;
}
