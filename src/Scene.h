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
	
	bool Load(const std::string& scene_file);
	bool LoadOBJ(const std::string& file, const int model_index);
	bool Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const;
};

#endif
