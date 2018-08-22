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
	glm::vec3 right;
	glm::vec3 up;
	float NEAR_PLANE;
	float FAR_PLANE;

	Camera(const glm::vec3& position, const glm::vec3& view_direction, const float vertical_fov, const float aspect_ratio);
	Ray GenerateRay(const float u, const float v) const;
};

#endif
