#ifndef SURFACEINTERACTION_H
#define SURFACEINTERACTION_H

#include "glm/vec3.hpp"

struct Ray;
struct Shape;

struct SurfaceInteraction
{
	Ray* ray;
	Shape* shape;
	glm::vec3 point;
	glm::vec3 normal;
};

#endif