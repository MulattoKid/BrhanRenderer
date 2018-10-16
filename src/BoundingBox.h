#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "glm/vec3.hpp"

struct Sphere;
struct Triangle;
struct Quad;

//Axis-Aligned Bounding Box
struct BoundingBox
{
	//Left-down-out
	glm::vec3 min;
	//Right-up-in
	glm::vec3 max;
	
	BoundingBox();
	BoundingBox(const glm::vec3& min, const glm::vec3& max);
	BoundingBox(const Sphere& s);
	BoundingBox(const Triangle& t);
	BoundingBox(const Quad& q);
	float SurfaceArea() const;
	int MaximumExtent() const;
	glm::vec3 Offset(const glm::vec3& p) const;
};

BoundingBox BoundingBoxUnion(const BoundingBox& b, const glm::vec3& p);
BoundingBox BoundingBoxUnion(const BoundingBox& b1, const BoundingBox& b2);

#endif
