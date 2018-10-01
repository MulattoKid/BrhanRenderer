#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "Math.h"
#include "RNG.h"
#include "Triangle.h"

Triangle::Triangle() : Shape(true)
{}

glm::vec3 Triangle::BarycentricCoefficients(const glm::vec3& point) const
{
	const float triangle_area = Area();
	const float triangle0_area = glm::length(glm::cross(point - v[0], v[1] - v[0])) / 2.0f;
	const float triangle1_area = glm::length(glm::cross(point - v[1], v[2] - v[1])) / 2.0f;
	//const float triangle2_area = triangle_area - triangle1_area - triangle0_area; //Not really needed
	
	const float lamda0 = triangle0_area / triangle_area;
	const float lamda1 = triangle1_area / triangle_area;
	const float lamda2 = 1.0f - lamda1 - lamda0;
	
	return glm::vec3(lamda0, lamda1, lamda2);
}

glm::vec3 Triangle::Normal(const glm::vec3& point) const
{
	const glm::vec3 lamdas = BarycentricCoefficients(point);
	return glm::normalize((lamdas.x * n[0]) + (lamdas.y * n[1]) + (lamdas.z * n[2]));
}

glm::vec2 Triangle::UV(const glm::vec3& point) const
{
	const glm::vec3 lamdas = BarycentricCoefficients(point);
	return glm::normalize((lamdas.x * uv[0]) + (lamdas.y * uv[1]) + (lamdas.z * uv[2]));
}

float Triangle::Area() const
{
	return glm::length(glm::cross(v[1] - v[0], v[2] - v[0])) / 2.0f;
}

//p.782
glm::vec3 Triangle::Sample(RNG& rng, const float u[2]) const
{
	const float sqrt_u0 = glm::sqrt(u[0]);
	const float lamda0 = 1.0f - sqrt_u0;
	const float lamda1 = u[1] * sqrt_u0;
	const float lamda2 = 1.0f - lamda1 - lamda0;
	
	return (lamda0 * v[0]) + (lamda1 * v[1]) + (lamda2 * v[2]);
}


bool Triangle::PointIn(const glm::vec3& point) const
{
	const float total_triangle_area_x2 = glm::length(glm::cross(v[1] - v[0], v[2] - v[0])); //Does not divide by two to avoid division [ (a/2)/(b/2) = a/b ]

	const glm::vec3 pv0 = v[0] - point;
	const glm::vec3 pv1 = v[1] - point;
	const glm::vec3 pv2 = v[2] - point;
	float pv0v1_triangle_area_x2 = glm::length(glm::cross(pv0, pv1)); //Does not divide by two to avoid division [ (a/2)/(b/2) = a/b ]
	float pv1v2_triangle_area_x2 = glm::length(glm::cross(pv1, pv2)); //Does not divide by two to avoid division [ (a/2)/(b/2) = a/b ]
	float pv2v0_triangle_area_x2 = glm::length(glm::cross(pv2, pv0)); //Does not divide by two to avoid division [ (a/2)/(b/2) = a/b ]
	
	if (pv0v1_triangle_area_x2 + pv1v2_triangle_area_x2 + pv2v0_triangle_area_x2 > total_triangle_area_x2)
	{
		return false;
	}
	return true;
}


//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
bool Triangle::Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const
{
	//Convert all to local math objects
	Vec3 ray_origin(ray->origin);
	Vec3 ray_dir(ray->dir);
	const glm::vec3 glm_v0v1 = v[1] - v[0];
	const glm::vec3 glm_v0v2 = v[2] - v[0];
	Vec3 v0(v[0]);
	Vec3 v0v1(glm_v0v1);
	Vec3 v0v2(glm_v0v2);
	static const Float ZERO(0.0f);
	static const Float ONE(1.0f);
	static const Float EPSILON(0.00001f);
	
	Vec3 pvec = Cross(ray_dir, v0v2);
	Float det = Dot(v0v1, pvec);
	/*if (double_sided)
	{
		if (Abs(det) < EPSILON) return false;
	}
	else
	{
		if (det < EPSILON) return false;
	}*/
	if (det < EPSILON) return false;
	Float invDet = ONE / det;

	Vec3 tvec = ray_origin - v0;
	Float u = Dot(tvec, pvec) * invDet;
	if (u < ZERO || u > ONE) return false;

	Vec3 qvec = Cross(tvec, v0v1);
	Float v = Dot(ray_dir, qvec) * invDet;
	if (v < ZERO || u + v > ONE) return false;

	Float t = Dot(v0v2, qvec) * invDet;
	if (t.f < ray->t && t.f >= t_min && t.f <= t_max)
	{
		ray->t = t.f;
		isect->shape = (Shape*)(this);
		return true;
	}

	return false;
}

bool Triangle::Intersect(Ray* ray, SurfaceInteraction* isect, const float t_less_than) const
{
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
	if (t < ray->t && t >= 0.0f && t < t_less_than)
	{
		ray->t = t;
		isect->shape = (Shape*)(this);
		return true;
	}

	return false;
}
