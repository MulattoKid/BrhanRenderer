#ifndef FRESNEL_H
#define FRESNEL_H

#include "glm/vec3.hpp"

struct Fresnel
{
	virtual glm::vec3 Evaluate() const = 0;
};

#endif