#ifndef MODEL_H
#define MODEL_H

#include "GlassMaterial.h"
#include "MatteMaterial.h"
#include "MirrorMaterial.h"
#include "MTL.h"
#include "PlasticMaterial.h"
#include "Quad.h"
#include "Shape.h"
#include "Sphere.h"
#include "SurfaceInteraction.h"
#include "TranslucentMaterial.h"
#include "Triangle.h"
#include <vector>

struct Model
{
	std::vector<Sphere> spheres;
	std::vector<Triangle> triangles;
	std::vector<Quad> quads;
	std::vector<Shape*> shapes;
	std::vector<MatteMaterial> matte_materials;
	std::vector<MirrorMaterial> mirror_materials;
	std::vector<PlasticMaterial> plastic_materials;
	std::vector<TranslucentMaterial> translucent_materials;
	std::vector<GlassMaterial> glass_materials;
	std::vector<Material*> materials;
	std::vector<MTL> mtls;
	bool has_uvs;
	
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const;
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_less_than) const;
};

#endif
