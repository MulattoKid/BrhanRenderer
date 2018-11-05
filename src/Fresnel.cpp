#include <algorithm>
#include "Fresnel.h"
#include "glm/common.hpp"
#include "glm/exponential.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "Logger.h"

Fresnel::~Fresnel()
{}

//Boundary between a dielectric and a condutor
//https://github.com/mmp/pbrt-v3/blob/master/src/core/reflection.cpp
glm::vec3 Conductor(const glm::vec3& wo, const glm::vec3& normal, const glm::vec3& eta_from, const glm::vec3& eta_to, const glm::vec3& k_to)
{
	//p.522 - see for reason about glm::abs
	float cos_theta_from = glm::abs(glm::dot(wo, normal));
	//For safety
	cos_theta_from = glm::clamp(cos_theta_from, -1.0f, 1.0f);
	glm::vec3 eta = eta_to / eta_from;
	glm::vec3 eta_k = k_to / eta_from;
	
	float cos_theta_from_2 = glm::pow(cos_theta_from, 2);
	float sin_theta_from_2 = 1.0f - cos_theta_from_2;
	glm::vec3 eta_2 = eta * eta;
	glm::vec3 eta_k_2 = eta_k * eta_k;
	
	glm::vec3 t_0 = eta_2 - eta_k_2 - sin_theta_from_2;
	glm::vec3 a_2_plus_b_2 = glm::sqrt(t_0 * t_0 + 4.0f * eta_2 * eta_k_2);
	glm::vec3 t_1 = a_2_plus_b_2 + cos_theta_from_2;
	glm::vec3 a = glm::sqrt(0.5f * (a_2_plus_b_2 + t_0));
	glm::vec3 t_2 = 2.0f * cos_theta_from * a;
	glm::vec3 r_perp = (t_1 - t_2) / (t_1 + t_2);
	
	glm::vec3 t_3 = a_2_plus_b_2 * cos_theta_from_2 + glm::pow(sin_theta_from_2, 2.0f);
	glm::vec3 t_4 = t_2 * sin_theta_from_2;
	glm::vec3 r_para = r_perp * (t_3 - t_4) / (t_3 + t_4);
	
	return 0.5f * (r_perp + r_para);
}

//p.519
glm::vec3 Dielectric(const glm::vec3& wo, const glm::vec3& normal, float eta_from, float eta_to)
{
	//Check which on which side of the medium the incident vector is
	float cos_theta_from = glm::dot(wo, normal);
	bool exiting = cos_theta_from <= 0.0f;
	if (exiting)
	{
		std::swap(eta_from, eta_to);
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
