#include "BSDF.h"
#include "Camera.h"
#include "glm/geometric.hpp"
#include "MemoryPool/MemoryPool.h"
#include "Ray.h"
#include "RayDifferential.h"
#include "Scene.h"
#include "Shape.h"
#include "SurfaceInteraction.h"

void SurfaceInteraction::Delete(MemoryPool* mem_pool, const int thread_id)
{
	if (bsdf != NULL)
	{
		bsdf->Delete(mem_pool, thread_id);
	}
}

//p.661
void SurfaceInteraction::ComputeDifferentials(const RayDifferential& ray, const unsigned int depth)
{
	if (depth != 0)
	{
		dpdx = dpdy = glm::vec3(0.0f);
		dudx = dudy = dvdx = dvdy = 0.0f;
		return;
	}
	
	//Compute intersection points between the offset rays and
	//the plane tangent to the intersection point given its normal:
	//ax + by + cz + d = 0
	//a = normal.x, b = normal.y, c = normal.z
	const glm::vec3 glm_point(point.x.f, point.y.f, point.z.f);
	float d = glm::dot(normal, glm_point);
	float tx = -(glm::dot(normal, ray.horizontal_offset_ray.origin) - d) / glm::dot(normal, ray.horizontal_offset_ray.dir);
	glm::vec3 px = ray.horizontal_offset_ray.origin + tx * ray.horizontal_offset_ray.dir;
	float ty = -(glm::dot(normal, ray.vertical_offset_ray.origin) - d) / glm::dot(normal, ray.vertical_offset_ray.dir);
	glm::vec3 py = ray.vertical_offset_ray.origin + ty * ray.vertical_offset_ray.dir;
	
	dpdx = px - glm_point;
	dpdy = py - glm_point;
	
}

void SurfaceInteraction::ComputeScatteringFunctions(MemoryPool* mem_pool, const int thread_id)
{
	shape->ComputeScatteringFunctions(this, mem_pool, thread_id);
}

glm::vec3 SurfaceInteraction::Le(const Scene& scene)
{
	if (shape->IsAreaLight())
	{
		return scene.area_lights[shape->area_light_index]->L(Point(), wo);
	}
	
	return glm::vec3(0.0f);
}
