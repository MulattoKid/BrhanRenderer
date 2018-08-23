#ifndef AREALIGHT_H
#define AREALIGHT_H

#include "Shape.h"

struct AreaLight
{
	Shape* shape;
	
	virtual glm::vec3 L(const glm::vec3& point, const glm::vec3& wo) const = 0;
	virtual glm::vec3 Power() const = 0;
	virtual glm::vec3 SampleLi() const = 0; //TODO Section 14.2
};

#endif
