#include "Camera.h"
#include "glm/gtc/constants.hpp"
#include "glm/trigonometric.hpp"

#include <stdio.h>

Camera::Camera(const glm::vec3& position, const glm::vec3& view_direction, const float vertical_fov, const float aspect_ratio)
{
	const float theta = (vertical_fov * glm::pi<float>()) / 180.0f; //Convert to radians
	const float lens_height = glm::tan(theta);
	const float lens_width = lens_height * aspect_ratio;
	const float lens_half_width = lens_width / 2.0f;
	const float lens_half_height = lens_height / 2.0f;
	
	this->top_left_corner = glm::vec3(-lens_half_width, lens_half_height, -1.0f);
	this->horizontal_end = glm::vec3(2.0f * lens_half_width, 0.0f, 0.0f);
	this->vertical_end = glm::vec3(0.0f, -2.0f * lens_half_height, 0.0f);
	this->position = position;
	this->view_direction = view_direction;
	this->NEAR_PLANE = 0.0001f;
	this->FAR_PLANE = 10000.0f;
}


Ray Camera::GenerateRay(const float u, const float v) const
{
	return Ray(position, top_left_corner + (u * horizontal_end) + (v * vertical_end) - position);
}
