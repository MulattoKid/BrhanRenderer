#ifndef MODEL_H
#define MODEL_H

#include "MTL.h"
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
	std::vector<MTL> mtls;
	bool has_uvs;
	
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const;
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_less_than) const;
};

#endif
