#include "Camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& view_direction)
{
	this->top_left_corner = glm::vec3(-2.0f, 1.0f, -1.0f);
	this->horizontal_end = glm::vec3(4.0f, 0.0f, 0.0f);
	this->vertical_end = glm::vec3(0.0f, -2.0f, 0.0f);
	this->position = position;
	this->view_direction = view_direction;
	this->NEAR_PLANE = 0.0001f;
	this->FAR_PLANE = 10000.0f;
}


Ray Camera::GenerateRay(const float u, const float v) const
{
	return Ray(position, top_left_corner + (u * horizontal_end) + (v * vertical_end) - position);
}
