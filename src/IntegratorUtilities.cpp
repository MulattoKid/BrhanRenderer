#include "BSDF.h"
#include "glm/geometric.hpp"
#include "IntegratorUtilities.h"
#include "RNG.h"

#include <stdio.h>

glm::vec3 UniformSampleAll()
{
	//TODO
	return glm::vec3(0.0f);
}

glm::vec3 UniformSampleOne(const Scene& scene, const SurfaceInteraction& isect)
{
	const unsigned int area_light_index = RNG::Uniform1D() * scene.area_lights.size();
	const AreaLight* area_light = scene.area_lights[area_light_index];
	float u_light[2], u_scattering[2];
	RNG::Uniform2D(u_light);
	RNG::Uniform2D(u_scattering);
	return EstimateDirect(scene, area_light, isect, u_light, u_scattering) * float(scene.area_lights.size());
}

glm::vec3 EstimateDirect(const Scene& scene, const AreaLight* area_light, const SurfaceInteraction& isect, const float u_light[2], float u_scattering[2])
{
	glm::vec3 Ld(0.0f); //Direct lighting

	//Sample light source
	glm::vec3 sample_point(0.0f), wi(0.0f);
	float light_pdf = 0.0f;
	glm::vec3 Li = area_light->SampleLi(isect, u_light, &sample_point, &wi, &light_pdf);
	//printf("Li: %f %f %f\n", Li.x, Li.y, Li.z);
	if (Li != glm::vec3(0.0f) && light_pdf > 0.0f) //Light has a probability of arriving to the point and some light does
	{
		glm::vec3 f = isect.bsdf->f(isect.ray->dir, wi, BxDFType(BSDF_ALL & ~BSDF_SPECULAR)) * glm::abs(glm::dot(isect.normal, wi));
		if (f != glm::vec3(0.0f)) //Some light is reflected from the intersection point
		{
			//Check for visibility to light
			Ray vis_ray(isect.point, wi);
			SurfaceInteraction light_isect;
			//HACK: This should probably be dealt with in a better way
			//The subtraction of 0.0001f make it so that an intersection with the light itself isn't detected
			if (!scene.Intersect(&vis_ray, &light_isect, glm::length(vis_ray.origin - sample_point) - 0.0001f))
			{
				float weight = 1.0f; //TODO: PowerHeuristic
				Ld += f * Li * weight / light_pdf;
			}
		}
	}
	
	//Sample BRDF of intersection point
	BxDFType sampled_type;
	float scattering_pdf = 0.0f;
	glm::vec3 f = isect.bsdf->Samplef(isect.ray->dir, u_scattering, BxDFType(BSDF_ALL & ~BSDF_SPECULAR), isect.normal, &wi, &scattering_pdf, &sampled_type);
	f *= glm::abs(glm::dot(isect.normal, wi));
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
	if (found_intersection)
	{
		if (light_isect.shape == area_light->shape)
		{
			Li = area_light->L(light_isect.point, -wi);
		}
	}
	else //TODO: Account for InifiteAreaLights that don't have geometry: p.861
	{}
	Ld += f * Li * weight / scattering_pdf;	

	return Ld;
}
