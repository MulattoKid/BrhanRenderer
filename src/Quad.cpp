#include "glm/geometric.hpp"
#include "Quad.h"
#include "RNG.h"
#include "Triangle.h"

glm::vec3 Quad::Normal(const glm::vec3& point) const
{
	//Check if the point lies within the first triangle making up the quad
	Triangle t;
	t.v[0] = v[0];
	t.v[1] = v[1];
	t.v[2] = v[2];
	if (t.PointIn(point))
	{
		return t.Normal(point);
	}
	else
	{
		//Change to the second triangle making up the quad
		t.v[0] = v[2];
		t.v[1] = v[3];
		t.v[2] = v[0];
		return t.Normal(point);
	}
}

glm::vec2 Quad::UV(const glm::vec3& point) const
{
	//Check if the point lies within the first triangle making up the quad
	Triangle t;
	t.v[0] = v[0];
	t.v[1] = v[1];
	t.v[2] = v[2];
	if (t.PointIn(point))
	{
		return t.UV(point);
	}
	else
	{
		//Change to the second triangle making up the quad
		t.v[0] = v[2];
		t.v[1] = v[3];
		t.v[2] = v[0];
		return t.UV(point);
	}
}

float Quad::Area() const
{
	const float triangle_v0v1v2_area = glm::length(glm::cross(v[0] - v[1], v[2] - v[1])) / 2.0f;
	const float triangle_v2v3v0_area = glm::length(glm::cross(v[2] - v[3], v[0] - v[3])) / 2.0f;
	return triangle_v0v1v2_area + triangle_v2v3v0_area;
}

glm::vec3 Quad::Sample(const float u[2]) const
{
	//Compute area of each triangle and entire quad
	const float triangle_v0v1v2_area = glm::length(glm::cross(v[0] - v[1], v[2] - v[1])) / 2.0f;
	const float triangle_v2v3v0_area = glm::length(glm::cross(v[2] - v[3], v[0] - v[3])) / 2.0f;
	const float total_quad_area = triangle_v0v1v2_area + triangle_v2v3v0_area;

	//Compute probability of choosing each of the two triangles
	const float triangle_v0v1v2_prob = triangle_v0v1v2_area / total_quad_area;
	//const float triangle_v2v3v4_prob = 1.0f - triangle_v0v1v2_prob; //Not really needed

	//Choose triangle
	Triangle tri;
	const float triangle_seed = RNG::Uniform1D();
	if (triangle_seed <= triangle_v0v1v2_prob)
	{
		tri.v[0] = v[0];
		tri.v[1] = v[1];
		tri.v[2] = v[2];
	}
	else
	{
		tri.v[0] = v[2];
		tri.v[1] = v[3];
		tri.v[2] = v[0];
	}
	
	return tri.Sample(u);
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
