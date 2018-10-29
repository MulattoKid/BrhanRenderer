#ifndef CAMERA_H
#define CAMERA_H

#include "glm/vec2.hpp"
#include "RayDifferential.h"

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
	float dx, dy; //Change in ray origins starting form the camera

	Camera(const glm::vec3& position, const glm::vec3& view_direction, const float vertical_fov, const float aspect_ratio);
	RayDifferential GenerateRayDifferential(const float u, const float v, const glm::vec2& sample_offset) const;
};

#endif
