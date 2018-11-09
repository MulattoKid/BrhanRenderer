#ifndef MICROFACETDISTRIBUTION_H
#define MICROFACETDISTRIBUTION_H

#include "glm/vec3.hpp"

struct MicrofacetDistribution
{
	virtual ~MicrofacetDistribution();
	virtual float D(const glm::vec3& wh, const glm::vec3& normal) const = 0;
	virtual float Lambda(const glm::vec3& w, const glm::vec3& normal) const = 0;
	float G1(const glm::vec3& w, const glm::vec3& normal) const;
	float G(const glm::vec3& wo, const glm::vec3& normal, const glm::vec3& wi) const;
	float Pdf(const glm::vec3& wo, const glm::vec3& wh, const glm::vec3& normal) const;
	virtual glm::vec3 SampleWh(const glm::vec3& wo, const glm::vec3& normal, const float u[2]) const = 0;
	static float RoughnessToAlpha(float roughness);
};

struct BeckmannDistribution : MicrofacetDistribution
{
	float alpha;

	BeckmannDistribution(float alpha);
	~BeckmannDistribution();
	float D(const glm::vec3& wh, const glm::vec3& normal) const;
	float Lambda(const glm::vec3& w, const glm::vec3& normal) const;
	glm::vec3 SampleWh(const glm::vec3& wo, const glm::vec3& normal, const float u[2]) const;
};

#endif
