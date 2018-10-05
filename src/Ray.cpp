#include "glm/geometric.hpp"
#include "Ray.h"

Ray::Ray()
{
	this->origin = glm::vec3(0.0f);
	this->dir = glm::vec3(0.0f);
}

Ray::Ray(const glm::vec3& origin, const glm::vec3& dir)
{
	this->origin = origin;
	this->dir = glm::normalize(dir);
	this->t = EFloat(std::numeric_limits<float>::max());
}

Vec3 Ray::AtError() const
{
	return Vec3(origin) + (t * Vec3(dir));
}

Ray SpawnRayWithOffsetNoFlip(const Vec3& origin, const glm::vec3 dir, const glm::vec3& normal)
{	
	glm::vec3 error(origin.x.err, origin.y.err, origin.z.err);
	float d = glm::dot(glm::abs(normal), glm::abs(error));
	glm::vec3 offset = d * normal;
	
	return Ray(glm::vec3(origin.x.f, origin.y.f, origin.z.f) + offset, dir);
}

//Makes sure that the offset goes along the correct side of the surface
//depending on the direction of the new ray
Ray SpawnRayWithOffset(const Vec3& origin, const glm::vec3 dir, const glm::vec3& normal)
{
	glm::vec3 normal_wi_side = normal;
	//Ray direction is on the opposite side of the normal for the surface it's spawned from
	if (glm::dot(dir, normal) < 0.0f)
	{
		normal_wi_side = -normal_wi_side;
	}
	
	glm::vec3 error(origin.x.err, origin.y.err, origin.z.err);
	float d = glm::dot(glm::abs(normal), glm::abs(error));
	glm::vec3 offset = d * normal_wi_side;
	
	return Ray(glm::vec3(origin.x.f, origin.y.f, origin.z.f) + offset, dir);
}

//Makes sure that the offset goes along the correct side of the surface
//depending on the incoming direction (wo) that created the intersection point
//the ray is being spawned from.
//Useful for when checking visibility between a a point on a surface and a point
//on a light, when the direction (dir) is given by sampling the light and not the 
//BSDF of the surface
Ray SpawnRayWithOffsetVisibility(const Vec3& origin, const glm::vec3 dir, const glm::vec3& normal, const glm::vec3& incoming_dir)
{
	glm::vec3 normal_wi_side = normal;
	//Ray direction is on the opposite side of the normal for the surface it's spawned from
	if (glm::dot(incoming_dir, normal) < 0.0f)
	{
		normal_wi_side = -normal_wi_side;
	}
	
	glm::vec3 error(origin.x.err, origin.y.err, origin.z.err);
	float d = glm::dot(glm::abs(normal), glm::abs(error));
	glm::vec3 offset = d * normal_wi_side;
	
	return Ray(glm::vec3(origin.x.f, origin.y.f, origin.z.f) + offset, dir);
}
