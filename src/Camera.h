#ifndef CAMERA_H
#define CAMERA_H

#include "glm/vec3.hpp"

struct Camera
{
	glm::vec3 position;
	glm::vec3 view_direction;
	float NEAR_PLANE;
	float FAR_PLANE;

	Camera(const glm::vec3& position, const glm::vec3& view_direction);
};

#endif
