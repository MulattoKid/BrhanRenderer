#include "glm/common.hpp"
#include "glm/exponential.hpp"
#include "ImageIO.h"
#include "Logger.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
//#define STBI_MSC_SECURE_CRT
#include "stb/stb_image_write.h"
#include <string>

float GammaCorrect(float v)
{
	if (v <= 0.0031308f) { return 12.92f * v; }
	else { return 1.055f * glm::pow(v, 1.0f / 2.4f) - 0.055f; }
}

unsigned char ToUnsignedChar(float v)
{
	float tmp = 255.0f * GammaCorrect(v) + 0.5f;
	tmp = glm::clamp(tmp, 0.0f, 255.0f);
	return (unsigned char)(tmp);
}

std::string ExtractFileNameOnly(const std::string& path)
{
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

void WriteImage(const float* result, const BrhanSystem& system)
{
	unsigned char* image = new unsigned char[system.render_width * system.render_height * 4];

	for (unsigned int y = 0; y < system.render_height; y++)
	{
		for (unsigned int x = 0; x < system.render_width; x++)
		{
			int result_idx = (y * system.render_width + x) * 3; 
			int image_idx = (y * system.render_width + x) * 4;
			for (int i = 0; i < 3; i++)
			{
				image[image_idx + i] = ToUnsignedChar(result[result_idx + i]);
			}
			image[image_idx+3] = 255;
		}
	}

	std::string filename = ExtractFileNameOnly(system.scene_file) + "@" + std::to_string(system.render_width) + "x" + std::to_string(system.render_height) + "_" + std::to_string(system.spp) + "SPP_";
	filename += std::to_string(system.max_depth) + "DEPTH_";
	if (system.integrator_type == IntegratorType::DIRECT_LIGHTING_INTEGRATOR)
	{
		filename += "DIRECT";
	}
	else if (system.integrator_type == IntegratorType::PATH_INTEGRATOR)
	{
		filename += "PATH";
	}
	filename += ".bmp";
	
	if (!stbi_write_bmp(filename.c_str(), system.render_width, system.render_height, 4, (void*)image))
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Error writing image\n");
	}
	
	delete[] image;
}
