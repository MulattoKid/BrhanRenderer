#ifndef BRHANSYSTEM_H
#define BRHANSYSTEM_H

#include "Camera.h"
#include <chrono>
#include "Integrator.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "MemoryPool/MemoryPool.h"
#include "PixelSampler.h"
#include "RNG.h"
#include "Scene.h"

struct ModelLoad
{
	std::string file;
	glm::mat4 translation;
	bool translation_active = false;
	glm::mat4 rotation;
	bool rotation_active = false;
	glm::mat4 scaling;
	bool scaling_active = false;
	
	std::string material = "";
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 reflectance;
	glm::vec3 transmittance;
	bool has_custom_material = false;
};

struct SphereLoad
{
	SphereLoad();

	glm::vec3 center;
	float radius;
	
	std::string material;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 reflectance;
	glm::vec3 transmittance;
};

struct BrhanSystem
{
	glm::vec3 camera_position;
	glm::vec3 camera_view_direction;
	float camera_vertical_fov;
	unsigned int film_width;
	unsigned int film_height;
	unsigned int spp;
	unsigned int max_depth;
	unsigned int save_intervals;
	bool save_intervals_enabled = false;
	Integrator* integrator;
	IntegratorType integrator_type;
	char* scene_file;
	std::string path;
	std::vector<ModelLoad> models;
	std::vector<SphereLoad> spheres;
	
	BrhanSystem(const int argc, char** argv, Camera** camera, Scene** scene, float** film, RNG** rngs, MemoryPool** mem_pool, PixelSampler** pixel_sampler);
	~BrhanSystem();
	std::string ExtractFileNameOnly() const;
	void LoadCamera(const std::string& line);
	void LoadIntegrator(const std::string& line);
	void LoadSaveIntervals(const std::string& line);
	void AddModel(const std::string& line);
	void AddSphere(const std::string& line);
	void LoadSceneFile(const std::string& scene_file);
	void UpdateProgress(unsigned int y, std::chrono::high_resolution_clock::time_point start_time, std::chrono::high_resolution_clock::time_point update_time, unsigned int save_interval = 0) const;
};

#endif
