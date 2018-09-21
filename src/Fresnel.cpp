#include <algorithm>
#include "Fresnel.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

Fresnel::~Fresnel()
{}

glm::vec3 Dielectric(const glm::vec3& wo, const glm::vec3& normal, float eta_from, float eta_to)
{
	const float cos_theta_from = glm::dot(wo, normal);
	//Check which on which side of the medium the incident vector is
	bool exiting = cos_theta_from <= 0.0f;
	if (exiting)
	{
		std::swap(eta_from, eta_to);
	}
	//Find sine of outgoing angle theta_t using Snell's Law
	//and sin²x + cos²x = 1
	const float sin_theta_from = glm::sqrt(float(1.0f - glm::pow(cos_theta_from, 2)));
	const float sin_theta_to = (eta_from / eta_to) * sin_theta_from;
	
	//Check for total internal reflection
	if (sin_theta_to >= 1.0f)
	{
		return glm::vec3(1.0f);
	}
	
	//Find cos_theta_t using sin²x + cos²x = 1
	const float cos_theta_to = glm::sqrt(float(1.0f - glm::pow(sin_theta_to, 2)));
	
	
	//Find parallel and perpendicular Fresnel reflectance
	const float r_parallel = ((eta_to * cos_theta_from) - (eta_from * cos_theta_to)) / ((eta_to * cos_theta_from) + (eta_from * cos_theta_to));
	const float r_perpendicular = ((eta_from * cos_theta_from) - (eta_to * cos_theta_to)) / ((eta_from * cos_theta_from) + (eta_to * cos_theta_to));
	
	return glm::vec3((glm::pow(r_parallel, 2) + glm::pow(r_perpendicular, 2)) / 2.0f);
}
