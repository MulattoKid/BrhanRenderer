#include "Camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& view_direction)
{
	this->position = position;
	this->view_direction = view_direction;
}