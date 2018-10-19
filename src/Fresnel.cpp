#include <algorithm>
#include "Fresnel.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "Logger.h"

Fresnel::~Fresnel()
{}

glm::vec3 Dielectric(const glm::vec3& wo, const glm::vec3& normal, float eta_from, float eta_to)
{
	//Check which on which side of the medium the incident vector is
	float cos_theta_from = glm::dot(wo, normal);
	bool exiting = cos_theta_from <= 0.0f;
	glm::vec3 normal_wo_side = normal;
	if (exiting)
	{
		std::swap(eta_from, eta_to);
		normal_wo_side *= -1;
		cos_theta_from = glm::abs(cos_theta_from);
	}
	
	//Find sine of outgoing angle theta_to using Snell's Law
	//and sin²x + cos²x = 1
	const float sin_theta_from = glm::sqrt(glm::max(float(1.0f - glm::pow(cos_theta_from, 2)), 0.0f));
	const float sin_theta_to = (eta_from * sin_theta_from) / eta_to;
	
	//Check for total internal/external reflection
	if (sin_theta_to >= 1.0f)
	{
		return glm::vec3(1.0f);
	}
	
	//Find cos_theta_to using sin²x + cos²x = 1
	const float cos_theta_to = glm::sqrt(glm::max(float(1.0f - glm::pow(sin_theta_to, 2)), 0.0f));
	
	
	//Find parallel and perpendicular Fresnel reflectance
	const float r_parallel = ((eta_to * cos_theta_from) - (eta_from * cos_theta_to)) / ((eta_to * cos_theta_from) + (eta_from * cos_theta_to));
	const float r_perpendicular = ((eta_from * cos_theta_from) - (eta_to * cos_theta_to)) / ((eta_from * cos_theta_from) + (eta_to * cos_theta_to));
	
	return glm::vec3((glm::pow(r_parallel, 2) + glm::pow(r_perpendicular, 2)) / 2.0f);
}
