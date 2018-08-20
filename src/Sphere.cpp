#include "glm/geometric.hpp"
#include "Sphere.h"

Sphere::Sphere(const glm::vec3& center, const float radius)
{
	this->center = center;
	this->radius = radius;
}

bool Sphere::Intersect(Ray* ray) const
{
	//t²dot(ray->dir, ray->dir) + 2tdot(ray->dir, ray->origin - center) + dot(ray->origin - center, ray->origin - center) - radius² = 0
	const float a = glm::dot(ray->dir, ray->dir);
	const glm::vec3 center_to_ray_origin = ray->origin - center;
	const float b = 2.0f * glm::dot(ray->dir, center_to_ray_origin);
	const float c = glm::dot(center_to_ray_origin, center_to_ray_origin) - glm::pow(radius, 2);
	
	const float discriminant = glm::pow(b, 2) - 4.0f * a * c;
	if (discriminant < 0.0f) { return false; }
	
	const float denomiator = 2.0f * a;
	const float t1 = (-b + discriminant) / denomiator;
	const float t2 = (-b - discriminant) / denomiator;
	const float t = glm::min(t1, t2);
	if (t <= 0.0f) { return false; }
	
	ray->t = t;
	return true;
}

glm::vec3 Sphere::Normal(const glm::vec3& point) const
{
	return glm::normalize(point - center);
}	
