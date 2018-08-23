#ifndef MODEL_H
#define MODEL_H

#include "Quad.h"
#include "Shape.h"
#include "Sphere.h"
#include "SurfaceInteraction.h"
#include "Triangle.h"
#include <vector>

struct Model
{
	std::vector<Sphere> spheres;
	std::vector<Triangle> triangles;
	std::vector<Quad> quads;
	std::vector<Shape*> shapes;
	bool has_uvs;
	
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const;
};

#endif
