#ifndef SCENE_H
#define SCENE_H

#include "AreaLight.h"
#include "DiffuseAreaLight.h"
#include "Model.h"
#include "Ray.h"
#include <string>
#include "SurfaceInteraction.h"
#include <vector>

struct Scene
{
	std::vector<Model> models;
	std::vector<DiffuseAreaLight> diffuse_area_lights;
	std::vector<AreaLight*> area_lights;
	
	Scene(const std::vector<std::string>& model_files);
	bool LoadOBJ(const char* file, const int model_index);
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const;
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_less_than) const;
};

#endif
