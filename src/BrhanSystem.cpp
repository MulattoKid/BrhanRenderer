#include "BrhanSystem.h"
#include <cstdlib>
#include "DirectLightingIntegrator.h"
#include <fstream>
#include <iostream>
#include "Logger.h"
#include <omp.h>
#include "PathIntegrator.h"
#include <string>

void BrhanSystem::LoadCamera(const std::string& line)
{
	static const std::string position_str = "position";
	glm::vec3 position(0.0f);
	bool found_position = false;
	static const std::string view_direction_str = "view_direction";
	glm::vec3 view_direction(0.0f);
	bool found_view_direction = false;
	static const std::string vertical_fov_str = "vertical_fov";
	float vertical_fov = 0.0f;
	bool found_vertical_fov = false;
	static const std::string width_str = "width";
	unsigned int width = 0;
	bool found_width = false;
	static const std::string height_str = "height";
	unsigned int height = 0;
	bool found_height = false;
	
	unsigned int index = 7; //Eat "Camera "
	while (index < line.length())
	{
		if (line.compare(index, position_str.length(), position_str) == 0)
		{
			index += 9; //Eat "position["
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				position[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			found_position = true;
		}
		else if (line.compare(index, view_direction_str.length(), view_direction_str) == 0)
		{
			index += 15; //Eat "view_direction["
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				view_direction[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			found_view_direction = true;
		}
		else if (line.compare(index, vertical_fov_str.length(), vertical_fov_str) == 0)
		{
			index += 13; //Eat "view_direction["
			unsigned int end = index + 1;
			while (line[end] != ']') { end++; }
			vertical_fov = std::stof(line.substr(index, end - index));
			index = end + 1; //+1 to eat space
			found_vertical_fov = true;
		}
		else if (line.compare(index, width_str.length(), width_str) == 0)
		{
			index += 6; //Eat "width["
			unsigned int end = index + 1;
			while (line[end] != ']') { end++; }
			width = std::stoi(line.substr(index, end - index));
			index = end + 1; //+1 to eat space
			found_width = true;
		}
		else if (line.compare(index, height_str.length(), height_str) == 0)
		{
			index += 7; //Eat "height["
			unsigned int end = index + 1;
			while (line[end] != ']') { end++; }
			height = std::stoi(line.substr(index, end - index));
			index = end + 1; //+1 to eat space
			found_height = true;
		}
		
		index++;
	}
	
	if (!found_position)
  	{
  		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to locate camera position\n");
  	}
  	if (!found_view_direction)
  	{
  		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to locate camera view direction\n");
  	}
  	if (!found_vertical_fov)
  	{
  		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to locate camera vertical FOV\n");
  	}
  	if (!found_width)
  	{
  		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to locate camera film width\n");
  	}
  	if (!found_height)
  	{
  		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to locate camera film height\n");
  	}
	
	camera_position = position;
	camera_view_direction = view_direction;
	camera_vertical_fov = vertical_fov;
	film_width = width;
	film_height = height;
}

void BrhanSystem::LoadIntegrator(const std::string& line)
{
	static const std::string type_str = "type";
	std::string type = "";
	bool found_type = false;
	static const std::string spp_str = "spp";
	bool found_spp = false;
	static const std::string max_depth_str = "max_depth";
	bool found_max_depth = false;
	
	unsigned int index = 11; //Eat "Integrator "
	while (index < line.length())
	{
		if (line.compare(index, type_str.length(), type_str) == 0)
		{
			index += 5; //Eat "type["
			unsigned int end = index + 1;
			while (line[end] != ']') { end++; }
			type = line.substr(index, end - index);
			if (type == "direct")
			{
				integrator_type = DIRECT_LIGHTING_INTEGRATOR;
				integrator = new DirectLightingIntegrator();
			}
			else if (type == "path")
			{
				integrator_type = PATH_INTEGRATOR;
				integrator = new PathIntegrator();
			}
			else
			{
				LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Integrator type %s is not supported\n", type.c_str());
			}
			found_type = true;
		}
		else if (line.compare(index, spp_str.length(), spp_str) == 0)
		{
			index += 4; //Eat "spp["
			unsigned int end = index + 1;
			while (line[end] != ']') { end++; }
			spp = std::stoi(line.substr(index, end - index));
			found_spp = true;
		}
		else if (line.compare(index, max_depth_str.length(), max_depth_str) == 0)
		{
			index += 10; //Eat "max_depth["
			unsigned int end = index + 1;
			while (line[end] != ']') { end++; }
			max_depth = std::stoi(line.substr(index, end - index));
			found_max_depth = true;
		}
	
		index ++;
	}
	
	if (!found_type)
  	{
  		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to locate integrator type\n");
  	}
  	if (!found_spp)
  	{
  		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to locate spp (samples-per-pixel)\n");
  	}
  	if (!found_max_depth)
  	{
  		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to locate max depth (number of bounces)\n");
  	}
	
	LOG_MESSAGE(true, "Loaded integrator:\n"
					  "\ttype %s\n"
					  "\tspp %lu\n"
					  "\tmax_depth %lu\n",
					  type.c_str(), spp, max_depth);
}

void BrhanSystem::AddModel(const std::string& line)
{
	unsigned int index = 11; //Eat "Model file["
	unsigned int end = index + 1;
	while (line[end] != ']') { end++; }
	model_files.push_back(line.substr(index, end - index).c_str());
}

void BrhanSystem::LoadSceneFile(const std::string& scene_file)
{
	std::ifstream file(scene_file.c_str());
  	if (!file.is_open())
  	{
  		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to open file %s\n", scene_file);
  	}
  	
  	//Parameters
  	static const std::string camera_str = "Camera";
  	bool found_camera = false;
  	static const std::string integrator_str = "Integrator";
  	bool found_integrator = false;
  	static const std::string model_str = "Model";
  	
  	std::string line;
  	while (std::getline(file, line))
  	{
  		if (line.length() == 0) { continue; } //Empty
  		else if (line.compare(0, camera_str.length(), camera_str) == 0)
  		{
  			LoadCamera(line);
  			found_camera = true;
  		}
  		else if (line.compare(0, integrator_str.length(), integrator_str) == 0)
  		{
  			LoadIntegrator(line);
  			found_integrator = true;
  		}
  		else if (line.compare(0, model_str.length(), model_str) == 0)
  		{
  			AddModel(line);
  		}
  	}
  	
  	file.close();
  	
  	if (!found_camera)
  	{
  		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to locate camera\n");
  	}
  	if (!found_integrator)
  	{
  		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to locate integrator\n");
  	}
}

BrhanSystem::BrhanSystem(const int argc, char** argv, Camera** camera, Scene** scene, float** film, RNG** rngs, PixelSampler** pixel_sampler)
{
	const unsigned int num_args = 2;
	const unsigned int arg_file = 1;
	
	if (argc != num_args)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "%lu arguments must be passed\n", num_args - 1);
	}
	
	scene_file = argv[arg_file];
	LoadSceneFile(scene_file);
	
	*camera = new Camera(camera_position, camera_view_direction, camera_vertical_fov, float(film_width) / float(film_height));
	*film = new float[film_width * film_height * 3];
	*rngs = new RNG[omp_get_max_threads()];
	*pixel_sampler = new PixelSampler(spp, film_width, film_height);
	*scene = new Scene(model_files);
}

BrhanSystem::~BrhanSystem()
{
	if (integrator_type == IntegratorType::DIRECT_LIGHTING_INTEGRATOR)
	{
		DirectLightingIntegrator* ptr = (DirectLightingIntegrator*)(integrator);
		delete ptr;
	}
}

void BrhanSystem::UpdateProgress(unsigned int y) const
{
	unsigned int total_effort = film_width * film_height;
	unsigned int progress = (unsigned int)((film_width * y) / float(total_effort) * 100.0f);
	std::string output;
	output.resize(100, ' ');
	for (unsigned int i = 0; i < progress; i++)
	{
		output[i] = '-';
	}
	LOG_MESSAGE(true, "\rRender progress (%lu\%): [%s]", progress, output.c_str());
}
