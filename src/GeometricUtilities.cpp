#include "GeometricUtilities.h"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/mat3x3.hpp"
#include "glm/trigonometric.hpp"
#include "Logger.h"

//https://math.stackexchange.com/questions/133177/finding-a-unit-vector-perpendicular-to-another-vector
glm::vec3 PickVectorPerpendicularTo(const glm::vec3& v)
{
	glm::vec3 nv = glm::normalize(v);
	
	//Special cases
	//All components are zero
	if (nv == glm::vec3(0.0f))
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Vector nv is glm::vec3(0.0f)\n");
	}
	//Two components are zero
	if (nv.x == 0.0f && nv.y == 0.0f) //nv=(0,0,1)
	{
		return glm::vec3(1.0f, 0.0f, 0.0f);
	}
	if (nv.x == 0.0f && nv.z == 0.0f) //nv=(0,1,0)
	{
		return glm::vec3(1.0f, 0.0f, 0.0f);
	}
	if (nv.y == 0.0f && nv.z == 0.0f) //nv=(1,0,0)
	{
		return glm::vec3(0.0f, 0.0f, 1.0f);
	}
	//One component is zero
	if (nv.x == 0.0f)
	{
		float b = 1.0f;
		float c = -nv.y / nv.z;
		return glm::normalize(glm::vec3(1.0f, b, c));
	}
	if (nv.y == 0.0f)
	{
		float a = 1.0f;
		float c = -nv.x / nv.z;
		return glm::normalize(glm::vec3(a, 1.0f, c));
	}
	if (nv.z == 0.0f)
	{
		float a = 1.0f;
		float b = -nv.x / nv.y;
		return glm::normalize(glm::vec3(a, b, 1.0f));
	}
	
	//No components are zero
	float a = 1.0f;
	float b = 1.0f;
	float c = (-nv.x - nv.y) / nv.z;
	return glm::normalize(glm::vec3(a, b, c));
}

inline glm::vec3 FlipNegativeZerosVec3(const glm::vec3& v)
{
	glm::vec3 f(v);
	if (f.x == -0.0f) { f.x = 0.0f; }
	if (f.y == -0.0f) { f.y = 0.0f; }
	if (f.z == -0.0f) { f.z = 0.0f; }
	return f;
}

//https://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d
glm::mat3 RotationToAlignAToB(const glm::vec3& a, const glm::vec3& b)
{
	if (FlipNegativeZerosVec3(b) == a) { return glm::mat3(1.0f); } //No rotation is required
	const glm::vec3 v = glm::cross(a, b);
	const float s = glm::length(v);
	const float c = glm::dot(a, b);
	const glm::mat3 m(glm::vec3(0.0f, v[2], -v[1]), glm::vec3(-v[2], 0.0f, v[0]), glm::vec3(v[1], -v[0], 0.0f));
	const glm::mat3 rotation = glm::mat3(1.0f) + m + ((m * m) * ((1.0f - c) / (s * s)));
	return rotation;
}

//https://answers.unity.com/questions/1456941/project-a-vector-onto-a-plane.html
//https://www.opengl.org/discussion_boards/showthread.php/159739-Projection-of-a-3d-vector-on-a-plane
glm::vec3 ProjectVectorOntoPlane(const glm::vec3& v, const glm::vec3& plane_normal)
{
	const glm::vec3 r = v - (glm::dot(plane_normal, v) * plane_normal);
	return glm::normalize(r);
}

bool SameHemisphere(const glm::vec3& a, const glm::vec3& b, const glm::vec3& normal)
{
	//Check if they are in the same hemisphere
	if (glm::dot(a, normal) * glm::dot(b, normal) > 0.0f) { return true; }
	return false;
}

float UniformHemispherePdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal)
{
	if (SameHemisphere(wo, wi, normal))
	{
		return glm::two_over_pi<float>();

	}
	return 0.0f;
}

glm::vec3 UniformSampleHemisphere(const float u[2], const glm::vec3& normal)
{	
	//Transform sampled point to be around supplied normal,
	//which specifies the hemisphere around which the sample is to be taken
	//https://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d
	//1) Create rotation matrix that rotates a point around an axis
	//	- This axis is the cross-product of the two normals
	//2) Multiply the sampled point with this matrix

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
	const glm::mat3 rotation = RotationToAlignAToB(sampled_space_normal, normal);
	return glm::normalize(rotation * sample_point);
}

float CosineHemispherePdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal)
{
	if (SameHemisphere(wo, wi, normal))
	{
		return glm::abs(glm::dot(normal, wi)) * glm::one_over_pi<float>();
	}
	return 0.0f;
}

//http://www.rorydriscoll.com/2009/01/07/better-sampling/
glm::vec3 CosineSampleHemisphere(const float u[2], const glm::vec3& normal)
{
	//TODO
	return glm::vec3(0.0f);
}
