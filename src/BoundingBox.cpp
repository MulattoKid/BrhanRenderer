#include "BoundingBox.h"
#include "glm/common.hpp"
#include "Sphere.h"
#include "Triangle.h"
#include "Quad.h"

BoundingBox::BoundingBox()
{}

BoundingBox::BoundingBox(const glm::vec3& min, const glm::vec3& max)
{
	this->min = min;
	this->max = max;
}

BoundingBox::BoundingBox(const Sphere& s)
{
	this->min = glm::vec3(s.center.x - s.radius, s.center.y - s.radius, s.center.z + s.radius);
	this->max = glm::vec3(s.center.x + s.radius, s.center.y + s.radius, s.center.z - s.radius);
}

BoundingBox::BoundingBox(const Triangle& t)
{
	float min_x = glm::min(glm::min(t.v[0].x, t.v[1].x), t.v[2].x);
	float min_y = glm::min(glm::min(t.v[0].y, t.v[1].y), t.v[2].y);
	float min_z = glm::min(glm::min(t.v[0].z, t.v[1].z), t.v[2].z);
	
	float max_x = glm::max(glm::max(t.v[0].x, t.v[1].x), t.v[2].x);
	float max_y = glm::max(glm::max(t.v[0].y, t.v[1].y), t.v[2].y);
	float max_z = glm::max(glm::max(t.v[0].z, t.v[1].z), t.v[2].z);
	
	this->min = glm::vec3(min_x, min_y, max_z);
	this->max = glm::vec3(max_x, max_y, min_z);
}

BoundingBox::BoundingBox(const Quad& q)
{
	float min_x = glm::min(glm::min(glm::min(q.v[0].x, q.v[1].x), q.v[2].x), q.v[3].x);
	float min_y = glm::min(glm::min(glm::min(q.v[0].y, q.v[1].y), q.v[2].y), q.v[3].y);
	float min_z = glm::min(glm::min(glm::min(q.v[0].z, q.v[1].z), q.v[2].z), q.v[3].z);
	
	float max_x = glm::max(glm::max(glm::max(q.v[0].x, q.v[1].x), q.v[2].x), q.v[3].x);
	float max_y = glm::max(glm::max(glm::max(q.v[0].y, q.v[1].y), q.v[2].y), q.v[3].y);
	float max_z = glm::max(glm::max(glm::max(q.v[0].z, q.v[1].z), q.v[2].z), q.v[3].z);
	
	this->min = glm::vec3(min_x, min_y, max_z);
	this->max = glm::vec3(max_x, max_y, min_z);
}
