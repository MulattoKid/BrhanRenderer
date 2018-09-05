#ifndef SURFACEINTERACTION_H
#define SURFACEINTERACTION_H

#include "glm/vec3.hpp"

struct BSDF;
struct Ray;
struct Shape;

struct SurfaceInteraction
{
	Ray* ray = NULL;
	Shape* shape = NULL;
	glm::vec3 point;
	glm::vec3 normal;
	BSDF* bsdf = NULL;
	
	~SurfaceInteraction();
	void ComputeScatteringFunctions();
};

#endif
