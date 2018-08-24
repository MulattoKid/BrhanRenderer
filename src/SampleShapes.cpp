#include "glm/gtc/constants.hpp"
#include "glm/trigonometric.hpp"
#include "glm/geometric.hpp"
#include "RNG.h"
#include "SampleShapes.h"

glm::vec3 UniformSampleUnitDisk(const float u[2])
{
	//TODO: should improve accoriding to p.776
	const float r = glm::sqrt(u[0]);
	const float theta = glm::two_pi<float>() * u[1];
	return glm::vec3(r * glm::cos(theta), r * glm::sin(theta), 0.0f);
}

glm::vec3 UniformSampleUnitDisk()
{
	float u[2];
	RNG::Uniform2D(u);
	return UniformSampleUnitDisk(u);
}
