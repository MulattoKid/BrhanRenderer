#include "BrhanSystem.h"
#include "BSDF.h"
#include <cstdlib>
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
#include "FresnelConductor.h"
#include "FresnelDielectric.h"
#include "FresnelNoOp.h"
#include <fstream>
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <limits>
#include "Logger.h"
#include "MicrofacetDistribution.h"
#include <omp.h>
#include "PathIntegrator.h"
#include <string>
#include <time.h>

SphereLoad::SphereLoad() : diffuse(0.0f),
						   specular(0.0f),
						   reflectance(0.0f),
						   transmittance(0.0f)
{}

std::string BrhanSystem::ExtractFileNameOnly() const
{
	const std::string path(scene_file);
	int file_ext_pos = -1;
	int slash_pos = -1;
	for (size_t i = 0; i < path.length(); i++)
	{
		if (path[i] == '.')
		{
			file_ext_pos = i;
		}
		else if (path[i] == '/')
		{
			slash_pos = i;
		}
	}
	
	unsigned int end = path.length();
	if (file_ext_pos == -1)
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "Input file does not have an extension\n");
	}
	else
	{
		end = file_ext_pos;
	}
	
	unsigned int start = 0;
	if (slash_pos != -1) 
	{
		start = slash_pos + 1;
	}
	
	return path.substr(start, end - start);
}

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
			if (type == "path")
			{
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

void BrhanSystem::LoadSaveIntervals(const std::string& line)
{
	unsigned int start_index = 14; //Eat 'SaveIntervals['
	unsigned int index = start_index;
	while (index < line.length())
	{
		if (line[index] == ']' || line[index] == ' ')
		{
			save_intervals = std::stoi(line.substr(start_index, index - start_index).c_str());
			if (save_intervals > 0)
			{	
  				save_intervals_enabled = true;
			}
			break;
		}
		index++;
	}
	
	LOG_MESSAGE(true, "Save intervals ENABLED: %lu\n", save_intervals);
}

void BrhanSystem::LoadGenDepthImage(const std::string& line)
{
	unsigned int start_index = 14; //Eat 'GenDepthImage['
	unsigned int index = start_index;
	while (index < line.length())
	{
		if (line[index] == ']' || line[index] == ' ')
		{
			std::string value = line.substr(start_index, index - start_index);
			if (value == "true")
			{
				gen_depth_image = true;
			}
			break;
		}
		index++;
	}
}

void BrhanSystem::AddModel(const std::string& line)
{
	ModelLoad model;
	static const std::string file_str = "file";
	bool found_file = false;
	static const std::string translate_str = "translate";
	bool found_translate = false;
	static const std::string rotate_str = "rotate";
	bool found_rotate = false;
	static const std::string scale_str = "scale";
	bool found_scale = false;
	
	static const std::string material_str = "material";
	bool found_material = false;
	static const std::string diffuse_str = "diffuse";
	bool found_diffuse = false;
	static const std::string specular_str = "specular";
	bool found_specular = false;
	static const std::string reflectance_str = "reflectance";
	bool found_reflectance = false;
	static const std::string transmittance_str = "transmittance";
	bool found_transmittance = false;
	
	unsigned int index = 6; //Eat "Model "
	while (index < line.length())
	{
		if (line.compare(index, file_str.length(), file_str) == 0)
		{
			index += 5; //Eat "file["
			unsigned int end = index + 1;
			while (line[end] != ']') { end++; }
			model.file = line.substr(index, end - index);
			index = end + 1;
			found_file = true;
		}
		else if (line.compare(index, translate_str.length(), translate_str) == 0)
		{
			index += 10; //Eat "translate["
			glm::vec3 translation_vec(0.0f);
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				translation_vec[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			model.translation = glm::translate(glm::mat4(1.0f), translation_vec);
			model.translation_active = true;
			found_translate = true;
		}
		else if (line.compare(index, rotate_str.length(), rotate_str) == 0)
		{
			index += 7; //Eat "rotate["
			glm::vec3 rotation_vec(0.0f);
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				rotation_vec[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			model.rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_vec.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model.rotation = glm::rotate(model.rotation, glm::radians(rotation_vec.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model.rotation = glm::rotate(model.rotation, glm::radians(rotation_vec.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model.rotation_active = true;
			found_rotate = true;
		}
		else if (line.compare(index, scale_str.length(), scale_str) == 0)
		{
			index += 6; //Eat "scale["
			glm::vec3 scaling_vec(0.0f);
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				scaling_vec[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			model.scaling = glm::scale(glm::mat4(1.0f), scaling_vec);
			model.scaling_active = true;
			found_scale = true;
		}
		else if (line.compare(index, material_str.length(), material_str) == 0)
		{
			index += 9; //Eat "material["
			unsigned int end = index + 1;
			while (line[end] != ']') { end++; }
			model.material = line.substr(index, end - index);
			found_material = true;
			model.has_custom_material = true;
		}
		else if (line.compare(index, diffuse_str.length(), diffuse_str) == 0)
		{
			index += 8; //Eat "diffuse["
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				model.diffuse[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			found_diffuse = true;
		}
		else if (line.compare(index, specular_str.length(), specular_str) == 0)
		{
			index += 9; //Eat "specular["
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				model.specular[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			found_specular = true;
		}
		else if (line.compare(index, reflectance_str.length(), reflectance_str) == 0)
		{
			index += 12; //Eat "reflectance["
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				model.reflectance[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			found_reflectance = true;
		}
		else if (line.compare(index, transmittance_str.length(), transmittance_str) == 0)
		{
			index += 14; //Eat "transmittance["
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				model.transmittance[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			found_transmittance = true;
		}
		
		index++;
	}
	
	if (!found_file)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find model file\n");
	}
	if (!found_translate)
	{
		model.translation = glm::mat4(1.0f);
	}
	if (!found_rotate)
	{
		model.rotation = glm::mat4(1.0f);
	}
	if (!found_scale)
	{
		model.scaling = glm::mat4(1.0f);
	}
	if (found_material)
	{
		if (model.material == "matte" && !found_diffuse)
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find diffuse spectrum of model %s on line: '%s'\n", model.file.c_str(), line.c_str());
		}
		if (model.material == "mirror" && !found_specular)
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find specular spectrum of model %s on line: '%s'\n", model.file.c_str(), line.c_str());
		}
		if (model.material == "plastic" && (!found_diffuse || !found_specular))
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find diffuse or specular spectrum of model %s on line: '%s'\n", model.file.c_str(), line.c_str());
		}
		if ((model.material == "copper" || model.material == "gold" || model.material == "aluminium" || model.material == "salt") && !found_specular)
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find specular spectrum of model %s on line: '%s'\n", model.file.c_str(), line.c_str());
		}
		if (model.material == "translucent" && !found_transmittance)
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find transmittance spectrum of model %s on line: '%s'\n", model.file.c_str(), line.c_str());
		}
		if (model.material == "water" && (!found_reflectance || !found_transmittance))
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find reflectance or transmittance spectrum of model %s on line: '%s'\n", model.file.c_str(), line.c_str());
		}
		if (model.material == "glass" && (!found_reflectance || !found_transmittance))
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find reflectance or transmittance spectrum of model %s on line: '%s'\n", model.file.c_str(), line.c_str());
		}
	}
	
	models.push_back(model);
}

void BrhanSystem::AddSphere(const std::string& line)
{
	SphereLoad sphere;
	static const std::string center_str = "center";
	bool found_center = false;
	static const std::string radius_str = "radius";
	bool found_radius = false;
	
	static const std::string material_str = "material";
	bool found_material = false;
	static const std::string diffuse_str = "diffuse";
	bool found_diffuse = false;
	static const std::string specular_str = "specular";
	bool found_specular = false;
	static const std::string reflectance_str = "reflectance";
	bool found_reflectance = false;
	static const std::string transmittance_str = "transmittance";
	bool found_transmittance = false;
	
	unsigned int index = 7; //Eat "Sphere "
	while (index < line.length())
	{
		if (line.compare(index, center_str.length(), center_str) == 0)
		{
			index += 7; //Eat "center["
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				sphere.center[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			found_center = true;
		}
		else if (line.compare(index, radius_str.length(), radius_str) == 0)
		{
			index += 7; //Eat "radius["
			unsigned int end = index + 1;
			while (line[end] != ']') { end++; }
			sphere.radius = std::stof(line.substr(index, end - index));
			found_radius = true;
		}
		else if (line.compare(index, material_str.length(), material_str) == 0)
		{
			index += 9; //Eat "material["
			unsigned int end = index + 1;
			while (line[end] != ']') { end++; }
			sphere.material = line.substr(index, end - index);
			found_material = true;
		}
		else if (line.compare(index, diffuse_str.length(), diffuse_str) == 0)
		{
			index += 8; //Eat "diffuse["
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				sphere.diffuse[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			found_diffuse = true;
		}
		else if (line.compare(index, specular_str.length(), specular_str) == 0)
		{
			index += 9; //Eat "specular["
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				sphere.specular[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			found_specular = true;
		}
		else if (line.compare(index, reflectance_str.length(), reflectance_str) == 0)
		{
			index += 12; //Eat "reflectance["
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				sphere.reflectance[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			found_reflectance = true;
		}
		else if (line.compare(index, transmittance_str.length(), transmittance_str) == 0)
		{
			index += 14; //Eat "transmittance["
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				sphere.transmittance[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			found_transmittance = true;
		}
	
		index++;
	}
	
	if (!found_center)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find center of sphere\n");
	}
	if (!found_radius)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find radius of sphere\n");
	}
	if (!found_material)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find material of sphere\n");
	}
	else
	{
		if (sphere.material == "matte" && !found_diffuse)
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find diffuse spectrum of sphere on line: '%s'\n", line.c_str());
		}
		if (sphere.material == "mirror" && !found_specular)
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find specular spectrum of sphere on line: '%s'\n", line.c_str());
		}
		if (sphere.material == "plastic" && (!found_diffuse || !found_specular))
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find diffuse or specular spectrum of sphere on line: '%s'\n", line.c_str());
		}
		if ((sphere.material == "copper" || sphere.material == "gold" || sphere.material == "aluminium" || sphere.material == "salt") && !found_specular)
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find specular spectrum of sphere on line: '%s'\n", line.c_str());
		}
		if (sphere.material == "translucent" && !found_transmittance)
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find transmittance spectrum of sphere on line: '%s'\n", line.c_str());
		}
		if (sphere.material == "water" && (!found_reflectance || !found_transmittance))
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find reflectance or transmittance spectrum of sphere on line: '%s'\n", line.c_str());
		}
		if (sphere.material == "glass" && (!found_reflectance || !found_transmittance))
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find reflectance or transmittance spectrum of sphere on line: '%s'\n", line.c_str());
		}
	}
	
	spheres.push_back(sphere);
}

void BrhanSystem::AddSphericalLight(const std::string& line)
{
	SphericalLightLoad sL;
	static const std::string centerStr = "center";
	bool foundCenter = false;
	static const std::string radiusStr = "radius";
	bool foundRadius = false;
	static const std::string emittanceStr = "emittance";
	bool foundEmittance = false;
	
	unsigned int index = 15; //Eat "SphericalLight "
	while (index < line.length())
	{
		if (line.compare(index, centerStr.length(), centerStr) == 0)
		{
			index += 7; //Eat "center["
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				sL.center[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			foundCenter = true;
		}
		else if (line.compare(index, radiusStr.length(), radiusStr) == 0)
		{
			index += 7; //Eat "radius["
			unsigned int end = index + 1;
			while (line[end] != ']') { end++; }
			sL.radius = std::stof(line.substr(index, end - index));
			index = end + 1; //+1 to eat space
			foundRadius = true;
		}
		else if (line.compare(index, emittanceStr.length(), emittanceStr) == 0)
		{
			index += 10; //Eat "emittance["
			for (int i = 0; i < 3; i++)
			{
				unsigned int end = index + 1;
				while (line[end] != ' ' && line[end] != ']') { end++; }
				sL.emittance[i] = std::stof(line.substr(index, end - index));
				index = end + 1; //+1 to eat space
			}
			foundEmittance = true;
		}
		
		index++;
	}
	
	if (!foundCenter)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find center for spherical light source");
	}
	if (!foundRadius)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find radius for spherical light source");
	}
	if (!foundEmittance)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to find emittance for spherical light source");
	}
	
	sphericalLights.push_back(sL);
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
  	static const std::string save_intervals_str = "SaveIntervals";
  	static const std::string gen_depth_image_str = "GenDepthImage";
  	static const std::string model_str = "Model";
  	static const std::string sphere_str = "Sphere";
  	static const std::string spheicalLight_str = "SphericalLight";
  	
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
  		else if (line.compare(0, save_intervals_str.length(), save_intervals_str) == 0)
  		{
  			LoadSaveIntervals(line);
  		}
  		else if (line.compare(0, gen_depth_image_str.length(), gen_depth_image_str) == 0)
  		{
  			LoadGenDepthImage(line);
  		}
  		else if (line.compare(0, model_str.length(), model_str) == 0)
  		{
  			AddModel(line);
  		}
  		else if (line.compare(0, sphere_str.length(), sphere_str) == 0)
  		{
  			AddSphere(line);
  		}
  		else if (line.compare(0, spheicalLight_str.length(), spheicalLight_str) == 0)
  		{
  			AddSphericalLight(line);
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
  	if (save_intervals > spp)
  	{
  		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "The requested number of save itnervals (%lu) is larger than the requested number of samples per pixel (%lu)\n", save_intervals, spp);
  	}
}

BrhanSystem::BrhanSystem(const int argc, char** argv, Camera** camera, Scene** scene, float** film, float** depth_film, RNG** rngs, PixelSampler** pixel_sampler)
{
	const unsigned int num_args = 2;
	const unsigned int arg_scene_file = 1;
	
	if (argc != num_args)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Only %lu are supported\n", num_args - 1);
	}
	
	if (strcmp(argv[1], "--help") == 0)
	{
		LOG_MESSAGE(true, "The only input parameter needed is the path to the scene description file\n");
	}
	
	scene_file = argv[arg_scene_file];
	path = "renders/";
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	int sec = t->tm_sec;
	int min = t->tm_min;
	int hour = t->tm_hour;
	int day = t->tm_mday;
	int month = t->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
	int year = t->tm_year + 1900; // Year is # years since 1900
	path += std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day) + "-" + std::to_string(hour) + ":" + std::to_string(min) + ":" + std::to_string(sec) + "-";
	path += ExtractFileNameOnly();
	filesystem::create_directory(path.c_str());
	LoadSceneFile(scene_file);
	
	*camera = new Camera(camera_position, camera_view_direction, camera_vertical_fov, float(film_width) / float(film_height));
	*film = new float[film_width * film_height * 3];
	*depth_film = new float[film_width * film_height];
	*rngs = new RNG[omp_get_max_threads()];
	*pixel_sampler = new PixelSampler(spp, film_width, film_height);
	*scene = new Scene(models, spheres, sphericalLights);
}

BrhanSystem::~BrhanSystem()
{
	PathIntegrator* ptr = (PathIntegrator*)(integrator);
	delete ptr;
}

void BrhanSystem::UpdateProgress(unsigned int y, std::chrono::high_resolution_clock::time_point start_time, std::chrono::high_resolution_clock::time_point update_time, unsigned int save_interval) const
{
	unsigned int total_effort = film_width * film_height;
	unsigned int progress = (unsigned int)((film_width * y) / float(total_effort) * 100.0f);
	if (save_intervals_enabled)
	{
		unsigned int total_work = 100 * save_intervals;
		unsigned int work_already_done = 100 * save_interval;
		progress += work_already_done;
		progress = (unsigned int)(progress / float(total_work) * 100.0f);
	}
	
	std::string output;
	output.resize(100, ' ');
	for (unsigned int i = 0; i < progress; i++)
	{
		output[i] = '-';
	}
	
	unsigned int s_remaining = std::numeric_limits<unsigned int>::max();
	unsigned int m_remaining = std::numeric_limits<unsigned int>::max();
	unsigned int h_remaining = std::numeric_limits<unsigned int>::max();
	if (progress > 0)
	{
		unsigned int ms_used = (unsigned int)(std::chrono::duration_cast<std::chrono::milliseconds>(update_time - start_time).count());
		unsigned int ms_per_percent = ms_used / progress;
		unsigned int ms_remaining_total = ms_per_percent * (100 - progress);
		unsigned int s_remaining_total = ms_remaining_total / 1000;
		unsigned int m_remaining_total = s_remaining_total / 60;
		unsigned int h_remaining_total = m_remaining_total / 60;
		
		h_remaining = h_remaining_total;
		m_remaining = m_remaining_total - (h_remaining * 60);
		s_remaining = s_remaining_total - (m_remaining_total * 60);
	}
	std::string remaining_time_str = "Estimated time remaining: " + std::to_string(h_remaining) + "h " + std::to_string(m_remaining) + "m " + std::to_string(s_remaining) + "s";
	remaining_time_str.resize(62, ' '); //Length of 'Estimated time remaining: 4294967295h 4294967295m 4294967295s'
	LOG_MESSAGE(true, "\rRender progress (%lu\%): [%s] %s", progress, output.c_str(), remaining_time_str.c_str());
}
