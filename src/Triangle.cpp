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
	const float triangle0_area = glm::max(glm::length(glm::cross(v[1] - point, v[2] - point)) / 2.0f, 0.0f);
	const float triangle1_area = glm::max(glm::length(glm::cross(v[0] - point, v[2] - point)) / 2.0f, 0.0f);
	//const float triangle2_area = triangle_area - triangle1_area - triangle0_area; //Not really needed
	
	const float lamda0 = triangle0_area / triangle_area;
	const float lamda1 = triangle1_area / triangle_area;
	const float lamda2 = glm::max(1.0f - lamda1 - lamda0, 0.0f);
	
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
	return (lamdas[0] * uv[0]) + (lamdas[1] * uv[1]) + (lamdas[2] * uv[2]);
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
	Vec3 v0(v[0]);
	Vec3 v1(v[1]);
	Vec3 v2(v[2]);
	Vec3 v0v1 = v1 - v0;
	Vec3 v0v2 = v2 - v0;
	static const EFloat ZERO(0.0f);
	static const EFloat ONE(1.0f);
	static const EFloat EPSILON(0.0f);
	
	Vec3 pvec = Cross(ray_dir, v0v2);
	EFloat det = Dot(v0v1, pvec);
	if (double_sided)
	{
		if (Abs(det) < EPSILON) return false;
	}
	else
	{
		if (det < EPSILON) return false;
	}
	EFloat invDet = ONE / det;

	Vec3 tvec = ray_origin - v0;
	EFloat u = Dot(tvec, pvec) * invDet;
	if (u < ZERO || u > ONE) return false;

	Vec3 qvec = Cross(tvec, v0v1);
	EFloat v = Dot(ray_dir, qvec) * invDet;
	if (v < ZERO || (u + v) > ONE) return false;

	EFloat t = Dot(v0v2, qvec) * invDet;
	if (t.f < ray->t && t.f >= t_min && t.f <= t_max)
	{
		ray->t = t;
		isect->shape = (Shape*)(this);
		return true;
	}

	return false;
}

bool Triangle::Intersect(Ray* ray, SurfaceInteraction* isect, const float t_less_than) const
{
	//Convert all to local math objects
	Vec3 ray_origin(ray->origin);
	Vec3 ray_dir(ray->dir);
	Vec3 v0(v[0]);
	Vec3 v1(v[1]);
	Vec3 v2(v[2]);
	Vec3 v0v1 = v1 - v0;
	Vec3 v0v2 = v2 - v0;
	static const EFloat ZERO(0.0f);
	static const EFloat ONE(1.0f);
	static const EFloat EPSILON(0.00001f);
	
	Vec3 pvec = Cross(ray_dir, v0v2);
	EFloat det = Dot(v0v1, pvec);
	if (double_sided)
	{
		if (Abs(det) < EPSILON) return false;
	}
	else
	{
		if (det < EPSILON) return false;
	}
	EFloat invDet = ONE / det;

	Vec3 tvec = ray_origin - v0;
	EFloat u = Dot(tvec, pvec) * invDet;
	if (u < ZERO || u > ONE) return false;

	Vec3 qvec = Cross(tvec, v0v1);
	EFloat v = Dot(ray_dir, qvec) * invDet;
	if (v < ZERO || u + v > ONE) return false;

	EFloat t = Dot(v0v2, qvec) * invDet;
	if (t < ray->t && t >= 0.0f && t < t_less_than)
	{
		ray->t = t;
		isect->shape = (Shape*)(this);
		return true;
	}

	return false;
}
