#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

struct Camera
{
	//Abstract concepts
	glm::vec3 top_left_corner;
	glm::vec3 horizontal_end;
	glm::vec3 vertical_end;
	//Scene concepts
	glm::vec3 position;
	glm::vec3 view_direction;
	float NEAR_PLANE;
	float FAR_PLANE;

	Camera(const glm::vec3& position, const glm::vec3& view_direction);
	Ray GenerateRay(const float u, const float v) const;
};

#endif
