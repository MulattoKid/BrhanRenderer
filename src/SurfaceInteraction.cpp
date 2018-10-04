#include "BSDF.h"
#include "Scene.h"
#include "Shape.h"
#include "SurfaceInteraction.h"

SurfaceInteraction::~SurfaceInteraction()
{
	if (bsdf != NULL)
	{
		delete bsdf;
	}
}

void SurfaceInteraction::ComputeScatteringFunctions()
{
	shape->ComputeScatteringFunctions(this);
}

glm::vec3 SurfaceInteraction::Le(const Scene& scene)
{
	if (shape->IsAreaLight())
	{
		return scene.area_lights[shape->area_light_index]->L(Point(), wo);
	}
	
	return glm::vec3(0.0f);
}
