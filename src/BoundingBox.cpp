#include "BoundingBox.h"
#include "glm/common.hpp"
#include "Sphere.h"
#include "Triangle.h"
#include "Quad.h"

BoundingBox::BoundingBox()
{
	float min = std::numeric_limits<float>::lowest();
	float max = std::numeric_limits<float>::max();
	this->min = glm::vec3(max, max, min);
	this->max = glm::vec3(min, min, max);
}

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

float BoundingBox::SurfaceArea() const
{
	//Diagonal
	glm::vec3 d = glm::abs(max - min);
	return 2.0f * (glm::abs(d.x * d.y) + glm::abs(d.x * d.z) + glm::abs(d.y * d.z));
}

int BoundingBox::MaximumExtent() const
{
	//Ranges
	float x_range = std::abs(max.x - min.x);
	float y_range = std::abs(max.y - min.y);
	float z_range = std::abs(max.z - min.z);
	
	if (x_range > y_range && x_range > z_range) { return 0; }
	else if (y_range > z_range)                 { return 1; }
	else                                        { return 2; }
}

glm::vec3 BoundingBox::Offset(const glm::vec3& p) const
{
	glm::vec3 min_p = p - min;
	glm::vec3 min_max = max - min;
	return glm::vec3(min_max.x == 0.0f ? 0.0f : min_p.x / min_max.x,
					 min_max.y == 0.0f ? 0.0f : min_p.y / min_max.y,
					 min_max.z == 0.0f ? 0.0f : min_p.z / min_max.z);
}

BoundingBox BoundingBoxUnion(const BoundingBox& b, const glm::vec3& p)
{
	return BoundingBox(
		glm::vec3(
			glm::min(b.min.x, p.x), glm::min(b.min.y, p.y), glm::max(b.min.z, p.z)
		),
		glm::vec3(
			glm::max(b.max.x, p.x), glm::max(b.max.y, p.y), glm::min(b.max.z, p.z)
		)
	);
}

BoundingBox BoundingBoxUnion(const BoundingBox& b1, const BoundingBox& b2)
{
	return BoundingBox(
		glm::vec3(
			glm::min(b1.min.x, b2.min.x), glm::min(b1.min.y, b2.min.y), glm::max(b1.min.z, b2.min.z)
		),
		glm::vec3(
			glm::max(b1.max.x, b2.max.x), glm::max(b1.max.y, b2.max.y), glm::min(b1.max.z, b2.max.z)
		)
	);
}
