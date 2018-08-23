#include "glm/geometric.hpp"
#include "Quad.h"
#include "Triangle.h"

glm::vec3 Quad::Normal(const glm::vec3& point) const
{
	//TODO: interpolate according to point

	return n[0];
}

glm::vec2 Quad::UV(const glm::vec3& point) const
{
	//TODO: interpolate according to point
	return glm::vec2(0.0f);
}

float Quad::Area() const
{
	float triangle_v0v1v2_area = glm::length(glm::cross(v[0] - v[1], v[2] - v[1])) / 2;
	float triangle_v2v3v0_area = glm::length(glm::cross(v[2] - v[3], v[0] - v[3])) / 2;
	return triangle_v0v1v2_area + triangle_v2v3v0_area;
}

bool Quad::Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const
{
	Triangle t1;
	t1.v[0] = v[0];
	t1.v[1] = v[1];
	t1.v[2] = v[2];
	t1.n[0] = n[0];
	t1.n[1] = n[1];
	t1.n[2] = n[2];
	t1.uv[0] = uv[0];
	t1.uv[1] = uv[1];
	t1.uv[2] = uv[2];
	if (t1.Intersect(ray, isect, t_min, t_max))
	{
		isect->shape = (Shape*)(this);
		return true;
	}

	Triangle t2;
	t2.v[0] = v[2];
	t2.v[1] = v[3];
	t2.v[2] = v[0];
	t2.n[0] = n[2];
	t2.n[1] = n[3];
	t2.n[2] = n[0];
	t2.uv[0] = uv[2];
	t2.uv[1] = uv[3];
	t2.uv[2] = uv[0]; 
	if (t2.Intersect(ray, isect, t_min, t_max))
	{
		isect->shape = (Shape*)(this);
		return true;
	}

	return false;
}
