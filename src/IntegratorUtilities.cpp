#include "BSDF.h"
#include "glm/geometric.hpp"
#include "IntegratorUtilities.h"
#include <math.h>
#include "Logger.h"
#include "RNG.h"

#include <stdio.h>

float PowerHeuristic(int nf, float f_pdf, int ng, float g_pdf)
{
	float f = nf * f_pdf;
	float g = ng * g_pdf;
	return (f * f) / (f * f + g * g);
}

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
	float light_pdf = 0.0f, scattering_pdf = 0.0f;
	float weight = 1.0f;
	glm::vec3 Li = area_light->SampleLi(rng, isect, u_light, &sample_point, &wi, &light_pdf);
	if (light_pdf > 0.0f && Li != glm::vec3(0.0f)) //Light has a probability of arriving at the point and some light does
	{
		glm::vec3 f = isect.bsdf->f(isect.wo, wi, BxDFType(BSDF_ALL & ~BSDF_SPECULAR));
		f *= glm::abs(glm::dot(isect.normal, wi));
		scattering_pdf = isect.bsdf->Pdf(isect.wo, wi, isect.normal, BxDFType(BSDF_ALL & ~BSDF_SPECULAR));
		if (f != glm::vec3(0.0f)) //Some light is reflected from the intersection point
		{
			//Check for visibility to light
			//TODO: I NEED TO MAKE A STABLE WAY OF MAKING SURE THAT A RAY WILL HIT THE GEOMETRY THAT IT SPAWNED ON
			//		USING DOUBLE-SIDED GEOMETRY IS A WASTEFUL TECHNIQUE
			Ray vis_ray(isect.point, wi);
			SurfaceInteraction light_isect;
			if (!scene.Intersect(&vis_ray, &light_isect, 0.0001f, 10000.0f) || light_isect.shape == area_light->shape)
			{		
				weight = PowerHeuristic(1, light_pdf, 1, scattering_pdf);
				Ld += (f * Li * weight) / light_pdf;
			}
		}
	}
	
	//Sample BRDF of intersection point
	BxDFType sampled_type;
	glm::vec3 f = isect.bsdf->Samplef(rng, isect.wo, u_scattering, BxDFType(BSDF_ALL & ~BSDF_SPECULAR), isect.normal, &wi, &scattering_pdf, &sampled_type);
	f *= glm::abs(glm::dot(isect.normal, wi));
	if (scattering_pdf > 0.0f && f != glm::vec3(0.0f)) //Light has a probability of being reflected from wi, via the point, and out wo, and some light will due to the material at the point
	{
		bool sampled_specular = sampled_type & BSDF_SPECULAR;
		if (!sampled_specular) //Multiple-importance sampling shouldn't be applied as there's only one direction that can be sampled
		{
			light_pdf = area_light->PdfLi(isect, wi);
			if (light_pdf == 0.0f)
			{
				return Ld;
			}
			weight = PowerHeuristic(1, scattering_pdf, 1, light_pdf);
		}
		Li = glm::vec3(0.0f); //Reset
		//Check if the sampled direction intersects the light source's geometry before anything else (direct light)
		Ray light_ray(isect.point, wi);
		SurfaceInteraction light_isect;
		if (scene.Intersect(&light_ray, &light_isect, 0.0001f, 10000.0f) && light_isect.shape == area_light->shape)
		{
			Li = area_light->L(light_isect.point, -wi);
		}
		else //TODO: Account for InifiteAreaLights that don't have geometry: p.861
		{}
		Ld += (f * Li * weight) / scattering_pdf;
	}

	return Ld;
}
