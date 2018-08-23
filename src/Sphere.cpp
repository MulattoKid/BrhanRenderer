#include "glm/geometric.hpp"
#include "Sphere.h"

Sphere::Sphere(const glm::vec3& center, const float radius)
{
	this->center = center;
	this->radius = radius;
}

glm::vec3 Sphere::Normal(const glm::vec3& point) const
{
	return glm::normalize(point - center);
}

glm::vec2 Sphere::UV(const glm::vec3& point) const
{
	//TODO
	return glm::vec2(0.0f);
}

bool Sphere::Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const
{
	//t²dot(ray->dir, ray->dir) + 2tdot(ray->dir, ray->origin - center) + dot(ray->origin - center, ray->origin - center) - radius² = 0
	const float a = glm::dot(ray->dir, ray->dir);
	const glm::vec3 center_to_ray_origin = ray->origin - center;
	const float b = 2.0f * glm::dot(ray->dir, center_to_ray_origin);
	const float c = glm::dot(center_to_ray_origin, center_to_ray_origin) - glm::pow(radius, 2);
	
	const float discriminant = glm::pow(b, 2) - 4.0f * a * c;
	if (discriminant <= 0.0f) { return false; }
	
	const float denomiator = 2.0f * a;
	const float t2 = (-b - glm::sqrt(discriminant)) / denomiator;
	if (t2 >= t_min && t2 <= t_max && t2 < ray->t)
	{
		ray->t = t2;
		isect->shape = (Shape*)(this);
		return true;
	}
	
	const float t1 = (-b + glm::sqrt(discriminant)) / denomiator;
	if (t1 >= t_min && t1 <= t_max && t1 < ray->t)
	{
		ray->t = t1;
		isect->shape = (Shape*)(this);
		return true;
	}
	
	return false;
}
