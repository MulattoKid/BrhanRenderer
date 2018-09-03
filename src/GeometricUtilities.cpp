#include "GeometricUtilities.h"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/mat3x3.hpp"
#include "glm/trigonometric.hpp"
#include <stdio.h>

bool SameHemisphere(const glm::vec3& a, const glm::vec3& b, const glm::vec3& normal)
{
	//Check if they are in the same hemisphere
	if (glm::dot(a, normal) * glm::dot(b, normal) > 0.0f) { return true; }
	return false;
}

glm::vec3 UniformSampleHemisphere(const float u[2], const glm::vec3& normal)
{	
	//Transform sampled point to be around supplied normal,
	//which specifies the hemisphere around which the sample is to be taken
	//https://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d
	//1) Create rotation matrix that rotates a point around an axis
	//	- This axis is the cross-product of the two normals
	//2) Multiply the smapled point with this matrix

	//Generate sample point (also a direction - no need to normalize)
	glm::vec3 sample_point;
	const float theta = glm::acos(u[0]);
	const float phi = glm::two_pi<float>() * u[1];
	sample_point.x = glm::sin(theta) * glm::cos(phi);
	sample_point.y = glm::sin(theta) * glm::sin(phi);
	sample_point.z = u[0];
	
	//This is the sampled hemisphere's normal because:
	// x's domain is [-1,1]
	// y's domain is [-1,1]
	// z's domain is [0,1]
	const glm::vec3 sampled_space_normal(0.0f, 0.0f, 1.0f);
	const glm::vec3 v = glm::cross(sampled_space_normal, normal);
	const float s = glm::length(v);
	const float c = glm::dot(sampled_space_normal, normal);
	const glm::mat3 m(glm::vec3(0.0f, v[2], -v[1]), glm::vec3(-v[2], 0.0f, v[0]), glm::vec3(v[1], -v[0], 0.0f));
	const glm::mat3 R = glm::mat3(1.0f) + m + ((m * m) * ((1.0f - c) / (s * s)));
	
	return glm::normalize(R * sample_point);
}

float UniformHemispherePdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal)
{
	if (SameHemisphere(-wo, wi, normal))
	{
		return glm::two_over_pi<float>();
	}
	return 0.0f;
}
