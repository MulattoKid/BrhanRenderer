#ifndef AREALIGHT_H
#define AREALIGHT_H

#include "glm/vec3.hpp"
#include "SurfaceInteraction.h"

struct Shape;

struct AreaLight
{
	Shape* shape;
	
	virtual glm::vec3 L(const glm::vec3& point, const glm::vec3& wo) const = 0;
	virtual glm::vec3 Power() const = 0;
	//Probability with respect to solid andle for SampleLi() to sample
	//the direction wi from the intersection point
	virtual float PdfLi(const SurfaceInteraction& isect, const glm::vec3& wi) const = 0;
	//u[2] is used to randomly sample the shape of the area light
	//pdf returns the probability that the sampling direction is chosen
	virtual glm::vec3 SampleLi(const SurfaceInteraction& isect, const float u[2], float* pdf, glm::vec3* wi) const = 0;
};

#endif