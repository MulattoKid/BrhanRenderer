#ifndef SURFACEINTERACTION_H
#define SURFACEINTERACTION_H

#include "glm/vec3.hpp"
#include "Math.h"

struct Camera;
struct BSDF;
struct MemoryPool;
struct Ray;
struct RayDifferential;
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
	//Differentials
	glm::vec3 dpdx, dpdy;
	float dudx, dvdx, dudy, dvdy;
	
	void Delete(MemoryPool* mem_pool, const int thread_id);
	inline glm::vec3 Point() const { return glm::vec3(point.x.f, point.y.f, point.z.f); }
	void ComputeDifferentials(const RayDifferential& ray, const unsigned int depth);
	void ComputeScatteringFunctions(MemoryPool* mem_pool, const int thread_id);
	glm::vec3 Le(const Scene& scene);
};

#endif
