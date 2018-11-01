#include "BSDF.h"
#include "Camera.h"
#include "glm/geometric.hpp"
#include "LinearAlgebra.h"
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
		dudx = dudy = dvdx = dvdy = 0.0f;
		return;
	}
	
	//Compute intersection points between the offset rays and
	//the plane tangent to the intersection point given its normal:
	//ax + by + cz + d = 0
	//a = normal.x, b = normal.y, c = normal.z
	const glm::vec3 p(point.x.f, point.y.f, point.z.f);
	float d = glm::dot(normal, p);
	float tx = -(glm::dot(normal, ray.horizontal_offset_ray.origin) - d) / glm::dot(normal, ray.horizontal_offset_ray.dir);
	glm::vec3 px = ray.horizontal_offset_ray.origin + tx * ray.horizontal_offset_ray.dir;
	float ty = -(glm::dot(normal, ray.vertical_offset_ray.origin) - d) / glm::dot(normal, ray.vertical_offset_ray.dir);
	glm::vec3 py = ray.vertical_offset_ray.origin + ty * ray.vertical_offset_ray.dir;
	
	//Choose the two dimensions for which the system of equations is to be solved
	int dim[2];
	if (glm::abs(normal.x) > glm::abs(normal.y) && glm::abs(normal.x) > glm::abs(normal.z))
	{
		dim[0] = 1;
		dim[1] = 2;
	}
	else if (glm::abs(normal.y) > glm::abs(normal.z))
	{
		dim[0] = 0;
		dim[1] = 2;
	}
	else
	{
		dim[0] = 0;
		dim[1] = 1;
	}
	//Initialize data for solving 2x2 system
	float A[2][2] = { { dpdu[dim[0]], dpdv[dim[0]] },
					  { dpdu[dim[1]], dpdv[dim[1]] } };
	float Bx[2] = { px[dim[0]] - p[dim[0]], px[dim[1]] - p[dim[1]] };
	float By[2] = { py[dim[0]] - p[dim[0]], py[dim[1]] - p[dim[1]] };
	//Solve systems
	if (!SolveLinearSystem2x2(A, Bx, &dudx, &dvdx))
	{
		dudx = dvdx = 0.0f;
	}
	if (!SolveLinearSystem2x2(A, By, &dudy, &dvdy))
	{
		dudy = dvdy = 0.0f;
	}
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
