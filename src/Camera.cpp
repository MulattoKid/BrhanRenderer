#include "Camera.h"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/trigonometric.hpp"
#include "Logger.h"
#include "SampleShapes.h"

#include <stdio.h>

Camera::Camera(const glm::vec3& position, const glm::vec3& view_direction, const float vertical_fov, const float aspect_ratio)
{
	const float theta = (vertical_fov * glm::pi<float>()) / 180.0f; //Convert to radians
	const float lens_half_height = glm::tan(theta / 2.0f);
	const float lens_half_width = lens_half_height * aspect_ratio;
	const float lens_width = lens_half_width * 2.0f;
	const float lens_height = lens_half_height * 2.0f;

	//Calculate the three vectors that define the camera	
	const glm::vec3 base_up(0.0f, 1.0f, 0.0f);
	this->position = position;
	this->view_direction = glm::normalize(view_direction);
	this->right = glm::normalize(glm::cross(this->view_direction, base_up));
	this->up = glm::normalize(glm::cross(this->right, this->view_direction));
	//Calculate top_left_corner
	//1) Start at camera's position
	//2) Go lens_half_width along the camera's left (-right) axis
	//3) Go lens_half_height along the camera's up axis
	//4) Go 1 unit along the camera's view_direction axis
	this->top_left_corner = this->position + (lens_half_width * (-this->right)) + (lens_half_height * this->up) + this->view_direction;
	//Go width of lense along the camera's right axis
	this->horizontal_end = lens_width * this->right;
	//Go height of lense along the camera's down (-up) axis
	this->vertical_end = lens_height * (-this->up);
	this->NEAR_PLANE = 0.0001f;
	this->FAR_PLANE = 10000.0f;
	
	LOG_MESSAGE(true, "Loaded camera:\n"
					  "\tposition [%f %f %f]\n"
					  "\tview_direction [%f %f %f]\n"
					  "\tvertical_fov [%f]\n",
					  position.x, position.y, position.z,
					  view_direction.x, view_direction.y, view_direction.z,
					  vertical_fov);
}

Ray Camera::GenerateRay(const float u, const float v) const
{
	return Ray(position, top_left_corner + (u * horizontal_end) + (v * vertical_end) - position);
}

RayDifferential Camera::GenerateRayDifferential(const float u, const float ux, const float v, const float vy, const glm::vec2& sample_offset) const
{
	RayDifferential rd;
	rd.primary_ray = Ray(position, top_left_corner + ((u + sample_offset.x) * horizontal_end) + ((v + sample_offset.y) * vertical_end) - position);
	rd.horizontal_offset_ray = Ray(position, top_left_corner + (ux * horizontal_end) + (v * vertical_end) - position);
	rd.vertical_offset_ray = Ray(position, top_left_corner + (u * horizontal_end) + (vy * vertical_end) - position);
	return rd;
}
