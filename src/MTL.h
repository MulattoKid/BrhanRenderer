#ifndef MTL_H
#define MTL_H

#include "glm/vec3.hpp"
#include <string>

struct MTL
{
	glm::vec3 ambient;
	std::string map_ka;
	glm::vec3 diffuse;
	std::string map_kd;
	glm::vec3 specular;
	std::string map_ks;
	glm::vec3 transmittance;
	glm::vec3 emission;
	float shininess;
	float index_of_refraction;
	float dissolve;
	int illumination_model;
};

#endif
