#ifndef MTL_H
#define MTL_H

#include "glm/vec3.hpp"

struct MTL
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 transmittance;
	glm::vec3 emission;
	float shininess;
	float index_of_refraction;
	float dissolve;
	int illumination_model;
};

#endif
