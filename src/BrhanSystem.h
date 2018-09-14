#ifndef BRHANSYSTEM_H
#define BRHANSYSTEM_H

#include "Camera.h"
#include "Integrator.h"
#include "glm/vec3.hpp"
#include "PixelSampler.h"
#include "RNG.h"
#include "Scene.h"

struct BrhanSystem
{
	glm::vec3 camera_position;
	glm::vec3 camera_view_direction;
	float camera_vertical_fov;
	unsigned int film_width;
	unsigned int film_height;
	unsigned int spp;
	unsigned int max_depth;
	Integrator* integrator;
	IntegratorType integrator_type;
	char* scene_file;
	std::vector<std::string> model_files;
	
	BrhanSystem(const int argc, char** argv, Camera** camera, Scene** scene, float** film, RNG** rngs, PixelSampler** pixel_sampler);
	~BrhanSystem();
	void LoadCamera(const std::string& line);
	void LoadIntegrator(const std::string& line);
	void AddModel(const std::string& line);
	void LoadSceneFile(const std::string& scene_file);
	void UpdateProgress(unsigned int y) const;
};

#endif
