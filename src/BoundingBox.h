#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "glm/vec3.hpp"

struct Sphere;
struct Triangle;
struct Quad;

//Axis-Aligned Bounding Box
struct BoundingBox
{
	BoundingBox();
	BoundingBox(const glm::vec3& min, const glm::vec3& max);
	BoundingBox(const Sphere& s);
	BoundingBox(const Triangle& t);
	BoundingBox(const Quad& q);
	
	//Left-down-out
	glm::vec3 min;
	//Right-up-in
	glm::vec3 max;
};

#endif
