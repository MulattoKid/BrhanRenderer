#include <algorithm>
#include "Fresnel.h"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

glm::vec3 Dielectric(const glm::vec3& wi, const glm::vec3& normal, float eta_i, float eta_t)
{
	//Find outgoing angle theta_t using Snell's Law
	const float theta_i = glm::dot(wi, normal);
	//Check which on which side of the medium the incident vector is
	//	theta_i >  0 -> outside
	//	theta_i <= 0 -> inside
	bool exiting = theta_i <= 0.0f;
	if (exiting)
	{
		std::swap(eta_i, eta_t);
	}
	const float theta_t = glm::asin((eta_i * glm::sin(theta_i) / eta_t));
	
	//Check for total internal reflection
	if (glm::sin(theta_t) >= 1.0f)
	{
		return glm::vec3(1.0f);
	}
	
	//Find parallel and perpendicular Fresnel reflectance
	const float cos_theta_i = glm::cos(theta_i);
	const float cos_theta_t = glm::cos(theta_t);
	const float r_parallel = (eta_t * cos_theta_i - eta_t * cos_theta_t) / (eta_t * cos_theta_i + eta_t * cos_theta_t);
	const float r_perpendicular = (eta_i * cos_theta_i - eta_t * cos_theta_t) / (eta_i * cos_theta_i + eta_t * cos_theta_t);
	
	return glm::vec3((glm::pow(r_parallel, 2) + glm::pow(r_perpendicular, 2)) / 2.0f);
}
