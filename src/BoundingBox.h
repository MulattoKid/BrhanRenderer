#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "glm/vec3.hpp"

struct Ray;
struct Sphere;
struct Triangle;

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
	const glm::vec3& operator[](int i) const
	{
		return i == 0 ? min : max;
	}
	glm::vec3& operator[](int i)
	{
		return i == 0 ? min : max;
	}
	float SurfaceArea() const;
	int MaximumExtent() const;
	glm::vec3 Offset(const glm::vec3& p) const;
	bool Intersect(const Ray& ray, const glm::vec3& inv_dir, const int dir_is_neg[3]) const;
};

BoundingBox BoundingBoxUnion(const BoundingBox& b, const glm::vec3& p);
BoundingBox BoundingBoxUnion(const BoundingBox& b1, const BoundingBox& b2);

#endif
