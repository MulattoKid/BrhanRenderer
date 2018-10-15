#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/trigonometric.hpp"
#include "Math.h"
#include "Sphere.h"

Sphere::Sphere(const glm::vec3& center, const float radius) : Shape(false)
{
	this->center = center;
	this->radius = radius;
	this->bb = BoundingBox(*this);
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

float Sphere::Area() const
{
	return 4.0f * glm::pi<float>() * glm::pow(radius, 2);
}

glm::vec3 Sphere::Sample(RNG& rng, const float u[2]) const
{
	const float param0 = 1.0f - glm::pow(u[0], 2);
	const float param1 = glm::two_pi<float>() * u[1];
	
	const float x = glm::sqrt(param0) * glm::cos(param1);
	const float y = glm::sqrt(param0) * glm::sin(param1);
	const float z = param0;
	glm::vec3 sample(x, y, z);
	
	sample += center;
	sample *= radius;
	
	return sample;
}

bool Sphere::Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const
{
	//Convert to local math objects
	Vec3 center_m(center);
	EFloat radius_m(radius);
	Vec3 ray_origin(ray->origin);
	Vec3 ray_dir(ray->dir);
	static const EFloat ZERO(0.0f);

	//t²dot(ray->dir, ray->dir) + 2tdot(ray->dir, ray->origin - center) + dot(ray->origin - center, ray->origin - center) - radius² = 0
	const EFloat a = Dot(ray_dir, ray_dir);
	const Vec3 center_to_ray_origin = ray_origin - center_m;
	const EFloat b = 2.0f * Dot(ray_dir, center_to_ray_origin);
	const EFloat c = Dot(center_to_ray_origin, center_to_ray_origin) - Pow(radius_m, 2.0f);
	
	const EFloat discriminant = Pow(b, 2.0f) - 4.0f * a * c;
	if (discriminant <= ZERO) { return false; }
	
	const EFloat denomiator = 2.0f * a;
	const EFloat t2 = (-b - Sqrt(discriminant)) / denomiator;
	if (t2.f >= t_min && t2.f <= t_max && t2.f < ray->t)
	{
		ray->t = t2.f;
		isect->shape = (Shape*)(this);
		return true;
	}
	
	const EFloat t1 = (-b + Sqrt(discriminant)) / denomiator;
	if (t1.f >= t_min && t1.f <= t_max && t1.f < ray->t)
	{
		ray->t = t1.f;
		isect->shape = (Shape*)(this);
		return true;
	}
	
	return false;
}

bool Sphere::Intersect(Ray* ray, SurfaceInteraction* isect, const float t_less_than) const
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
	if (t2 >= 0.0f && t2 <= t_less_than && t2 < ray->t)
	{
		ray->t = t2;
		isect->shape = (Shape*)(this);
		return true;
	}
	
	const float t1 = (-b + glm::sqrt(discriminant)) / denomiator;
	if (t1 >= 0.0f && t1 < t_less_than && t1 < ray->t)
	{
		ray->t = t1;
		isect->shape = (Shape*)(this);
		return true;
	}
	
	return false;
}
