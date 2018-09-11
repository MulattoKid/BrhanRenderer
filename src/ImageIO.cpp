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

	std::string filename = std::to_string(system.render_width) + "x" + std::to_string(system.render_height) + "_" + std::to_string(system.spp) + "SPP_";
	if (system.integrator_type == IntegratorType::DIRECT_LIGHTING_INTEGRATOR)
	{
		filename += "DIRECT";
	}
	else if (system.integrator_type == IntegratorType::PATH_INTEGRATOR)
	{
		filename += "PATH";
	}
	filename += "_" + std::to_string(system.depth) + "DEPTH.bmp";
	
	if (!stbi_write_bmp(filename.c_str(), system.render_width, system.render_height, 4, (void*)image))
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Error writing image\n");
	}
	
	delete[] image;
}
