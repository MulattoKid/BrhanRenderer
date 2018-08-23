#include "glm/geometric.hpp"
#include "Triangle.h"

glm::vec3 Triangle::Normal(const glm::vec3& point) const
{
	//TODO: interpolate according to point

	return n[0];
}

glm::vec2 Triangle::UV(const glm::vec3& point) const
{
	//TODO: interpolate according to point
	return glm::vec2(0.0f);
}

bool Triangle::Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const
{
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
	glm::vec3 v0v1 = v[1] - v[0];
	glm::vec3 v0v2 = v[2] - v[0];
	glm::vec3 pvec = glm::cross(ray->dir, v0v2);
	float det = glm::dot(v0v1, pvec);
	if (det < 0.00001f) return false;
	float invDet = 1 / det;

	glm::vec3 tvec = ray->origin - v[0];
	float u = glm::dot(tvec, pvec) * invDet;
	if (u < 0 || u > 1) return false;

	glm::vec3 qvec = glm::cross(tvec, v0v1);
	float v = glm::dot(ray->dir, qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	float t = glm::dot(v0v2, qvec) * invDet;
	if (t < ray->t && t >= t_min && t <= t_max) //Should this last check be here??? TODO
	{
		ray->t = t;
		isect->shape = (Shape*)(this);
		return true;
	}

	return false;
}
