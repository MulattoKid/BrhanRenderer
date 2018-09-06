#include "BSDF.h"
#include "glm/geometric.hpp"
#include "IntegratorUtilities.h"
#include "Logger.h"
#include "RNG.h"

#include <stdio.h>

glm::vec3 UniformSampleAll()
{
	//TODO
	return glm::vec3(0.0f);
}

glm::vec3 UniformSampleOne(const Scene& scene, const SurfaceInteraction& isect, RNG& rng)
{
	const unsigned int area_light_index = rng.Uniform1D() * scene.area_lights.size();
	const AreaLight* area_light = scene.area_lights[area_light_index];
	float u_light[2], u_scattering[2];
	rng.Uniform2D(u_light);
	rng.Uniform2D(u_scattering);
	
	return EstimateDirect(scene, rng, area_light, isect, u_light, u_scattering) * float(scene.area_lights.size());
}

glm::vec3 EstimateDirect(const Scene& scene, RNG& rng, const AreaLight* area_light, const SurfaceInteraction& isect, const float u_light[2], float u_scattering[2])
{
	glm::vec3 Ld(0.0f); //Direct lighting

	//Sample light source
	glm::vec3 sample_point(0.0f), wi(0.0f);
	float light_pdf = 0.0f;
	glm::vec3 Li = area_light->SampleLi(rng, isect, u_light, &sample_point, &wi, &light_pdf);
	//LOG_MESSAGE(true, "AreaLight: %p\n", area_light);
	//LOG_MESSAGE(true, "SamplePoint: %f %f %f\n", sample_point.x, sample_point.y, sample_point.z);
	//LOG_MESSAGE(true, "Wi: %f %f %f\n", wi.x, wi.y, wi.z);
	if (light_pdf > 0.0f && Li != glm::vec3(0.0f)) //Light has a probability of arriving to the point and some light does
	{
		glm::vec3 f = isect.bsdf->f(isect.ray->dir, wi, BxDFType(BSDF_ALL & ~BSDF_SPECULAR));
		f *= glm::abs(glm::dot(isect.normal, wi));
		//LOG_MESSAGE(true, "F: %f %f %f\n", f.x, f.y, f.z);
		if (f != glm::vec3(0.0f)) //Some light is reflected from the intersection point
		{
			//Check for visibility to light
			Ray vis_ray(isect.point, wi);
			SurfaceInteraction light_isect;
			//HACK: This should probably be dealt with in a better way
			//The subtraction of 0.0001f make it so that an intersection with the light itself isn't detected
			if (!scene.Intersect(&vis_ray, &light_isect, glm::length(vis_ray.origin - sample_point) - 0.0001f))
			{
				const float weight = 1.0f; //TODO: PowerHeuristic
				Ld += f * Li * weight / light_pdf;
				//LOG_MESSAGE(true, "Ld: %f %f %f\n", Ld.x, Ld.y, Ld.z);
			}
		}
	}
	
	//Sample BRDF of intersection point
	/*BxDFType sampled_type;
	float scattering_pdf = 0.0f;
	glm::vec3 f = isect.bsdf->Samplef(rng, isect.ray->dir, u_scattering, BxDFType(BSDF_ALL & ~BSDF_SPECULAR), isect.normal, &wi, &scattering_pdf, &sampled_type);
	f *= glm::abs(glm::dot(isect.normal, wi));
	if (f != glm::vec3(0.0f) && scattering_pdf > 0.0f)
	{
		bool sampled_specular = sampled_type & BSDF_SPECULAR;
		float weight = 1.0f;
		if (!sampled_specular) //Multiple-importance sampling shouldn't be applied as there's only one direction that can be sampled
		{
			light_pdf = area_light->PdfLi(isect, wi);
			if (light_pdf == 0.0f)
			{
				return Ld;
			}
			weight = 1.0f; //TODO: PowerHeuristic
		}
		//Check if the sampled direction intersects the light source's geometry
		Ray light_ray(isect.point, wi);
		SurfaceInteraction light_isect;
		bool found_intersection = scene.Intersect(&light_ray, &light_isect, 0.0001f, 10000.0f);
		Li = glm::vec3(0.0f);
		if (found_intersection)
		{
			if (light_isect.shape == area_light->shape)
			{
				Li = area_light->L(light_isect.point, -wi);
			}
		}
		else //TODO: Account for InifiteAreaLights that don't have geometry: p.861
		{}
		if (Li != glm::vec3(0.0f))
		{
			Ld += f * Li * weight / scattering_pdf;
		}
	}*/

	return Ld;
}
