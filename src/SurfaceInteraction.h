#ifndef SURFACEINTERACTION_H
#define SURFACEINTERACTION_H

#include "glm/vec3.hpp"

struct BSDF;
struct Ray;
struct Scene;
struct Shape;

struct SurfaceInteraction
{
	Ray* ray = NULL;
	Shape* shape = NULL;
	glm::vec3 point;
	glm::vec3 normal;
	glm::vec3 wo; //Points away from point
	BSDF* bsdf = NULL;
	
	~SurfaceInteraction();
	void ComputeScatteringFunctions();
	glm::vec3 Le(const Scene& scene);
};

#endif
