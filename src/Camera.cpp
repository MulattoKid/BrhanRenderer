#include "Camera.h"
#include "glm/geometric.hpp"
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
	
	/*const glm::vec3 position(0.0f);
	const glm::vec3 base_view_direction(0.0f, 0.0f, -1.0f);
	const glm::vec3 base_right = glm::cross(base_view_direction, base_up); //glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 base_top_left_corner = glm::vec3(-lens_half_width, lens_half_height, -1.0f);
	const glm::vec3 base_horizontal_end = glm::vec3(2.0f * lens_half_width, 0.0f, 0.0f);
	const glm::vec3 base_vertical_end = glm::vec3(0.0f, -2.0f * lens_half_height, 0.0f);*/

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
	
	printf("ViewDirection: %f %f %f\n", this->view_direction.x, this->view_direction.y, this->view_direction.z);
	printf("Right: %f %f %f\n", this->right.x, this->right.y, this->right.z);
	printf("Up: %f %f %f\n", this->up.x, this->up.y, this->up.z);
	const glm::vec3 base_top_left_corner = glm::vec3(-lens_half_width, lens_half_height, -1.0f);
	printf("BaseTopLeftCorner: %f %f %f\n", base_top_left_corner.x, base_top_left_corner.y, base_top_left_corner.z);
	printf("TopLeftCorner: %f %f %f\n", this->top_left_corner.x, this->top_left_corner.y, this->top_left_corner.z);
	printf("HorizontalEnd: %f %f %f\n", this->horizontal_end.x, this->horizontal_end.y, this->horizontal_end.z);
	printf("VerticalEnd: %f %f %f\n", this->vertical_end.x, this->vertical_end.y, this->vertical_end.z);
}


Ray Camera::GenerateRay(const float u, const float v) const
{
	return Ray(position, top_left_corner + (u * horizontal_end) + (v * vertical_end) - position);
}
