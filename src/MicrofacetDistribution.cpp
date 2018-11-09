#include "glm/common.hpp"
#include "glm/exponential.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/mat3x3.hpp"
#include "glm/trigonometric.hpp"
#include "Logger.h"
#include <math.h>
#include "MicrofacetDistribution.h"

glm::mat3 RotationToAlignAToB(const glm::vec3& a, const glm::vec3& b);

MicrofacetDistribution::~MicrofacetDistribution()
{}

float MicrofacetDistribution::G1(const glm::vec3& w, const glm::vec3& normal) const
{
	return 1.0f / (1.0f + Lambda(w, normal));
}

float MicrofacetDistribution::G(const glm::vec3& wo, const glm::vec3& normal, const glm::vec3& wi) const
{
	return 1.0f / (1.0f + Lambda(wo, normal) + Lambda(wi, normal));
}

float MicrofacetDistribution::Pdf(const glm::vec3& wo, const glm::vec3& wh, const glm::vec3& normal) const
{
	return D(wh, normal) * glm::abs(glm::dot(wh, normal));
}

float MicrofacetDistribution::RoughnessToAlpha(float roughness) {
    roughness = glm::max(roughness, float(1e-3));
    float x = glm::log(roughness);
    return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
           0.000640711f * x * x * x * x;
}

BeckmannDistribution::BeckmannDistribution(float alpha)
{
	this->alpha = alpha;
}

BeckmannDistribution::~BeckmannDistribution()
{}

//p.539
//http://www.reedbeta.com/blog/hows-the-ndf-really-defined/
float BeckmannDistribution::D(const glm::vec3& wh, const glm::vec3& normal) const
{
	float theta = glm::acos(glm::dot(wh, normal));
	float tan_theta_2 = glm::pow(glm::tan(theta), 2);
	if (std::isinf(tan_theta_2))
	{
		return 0.0f;
	}
	
	float cos_theta_4 = glm::pow(glm::dot(wh, normal), 4);
	float alpha_2 = alpha * alpha;
	return glm::exp(-tan_theta_2 / alpha_2) / (glm::pi<float>() * alpha_2 * cos_theta_4);
}

float BeckmannDistribution::Lambda(const glm::vec3& w, const glm::vec3& normal) const
{
	float theta = glm::acos(glm::dot(normal, w));
	float tan_theta_abs = glm::abs(glm::tan(theta));
	if (std::isinf(tan_theta_abs))
	{
		return 0.0f;
	}
	
	float a = 1.0f / (alpha * tan_theta_abs);
	if (a >= 1.6f)
	{
		return 0.0f;
	}
	return (1.0f - 1.259f * a + 0.396f * a * a) / (3.535f * a + 2.181f * a * a);
}

//p.809
glm::vec3 BeckmannDistribution::SampleWh(const glm::vec3& wo, const glm::vec3& normal, const float u[2]) const
{
	float log_sample = glm::log(u[0]);
	if (std::isinf(log_sample)) { log_sample = 0.0f; }
	float tan_theta_2 = -alpha * alpha * log_sample;
	float phi = u[1] * 2.0f * glm::pi<float>();
		
	//Map spherical cooridnates to a direction
	float cos_theta = 1.0f / glm::sqrt(1.0f + tan_theta_2);
	float sin_theta = glm::sqrt(glm::max(0.0f, 1.0f - cos_theta * cos_theta));
	glm::vec3 wh(sin_theta * glm::cos(phi), sin_theta * glm::sin(phi), cos_theta);
		
	//This is the sampled hemisphere's normal because:
	// x's domain is [-1,1]
	// y's domain is [-1,1]
	// z's domain is [0,1]
	const glm::vec3 sampled_space_normal(0.0f, 0.0f, 1.0f);
	const glm::mat3 rotation = RotationToAlignAToB(sampled_space_normal, normal);
	return glm::normalize(rotation * wh);
}
