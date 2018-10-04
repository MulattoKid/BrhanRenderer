#ifndef SURFACEINTERACTION_H
#define SURFACEINTERACTION_H

#include "glm/vec3.hpp"
#include "Math.h"

struct BSDF;
struct Ray;
struct Scene;
struct Shape;

struct SurfaceInteraction
{
	Ray* ray = NULL;
	Shape* shape = NULL;
	Vec3 point;
	glm::vec3 normal;
	glm::vec3 wo; //Points away from point
	BSDF* bsdf = NULL;
	
	~SurfaceInteraction();
	inline glm::vec3 Point() const { return glm::vec3(point.x.f, point.y.f, point.z.f); }
	void ComputeScatteringFunctions();
	glm::vec3 Le(const Scene& scene);
};

#endif
