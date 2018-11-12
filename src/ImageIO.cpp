#include "glm/common.hpp"
#include "glm/exponential.hpp"
#include "ImageIO.h"
#include "Logger.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
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

void WriteImage(const float* film, const BrhanSystem& system, const unsigned int interval)
{
	unsigned char* image = new unsigned char[system.film_width * system.film_height * 4];

	for (unsigned int y = 0; y < system.film_height; y++)
	{
		for (unsigned int x = 0; x < system.film_width; x++)
		{
			int film_idx = (y * system.film_width + x) * 3; 
			int image_idx = (y * system.film_width + x) * 4;
			for (int i = 0; i < 3; i++)
			{
				image[image_idx + i] = ToUnsignedChar(film[film_idx + i]);
			}
			image[image_idx+3] = 255;
		}
	}

	std::string filename = system.path + "/" + std::to_string(system.film_width) + "x" + std::to_string(system.film_height) + "_" + std::to_string(system.spp) + "SPP_" + std::to_string(system.max_depth) + "DEPTH";
	if (system.save_intervals_enabled)
	{
		filename += "_" + std::to_string(interval) + "interval";
	}
	filename += ".bmp";
	
	if (!stbi_write_bmp(filename.c_str(), system.film_width, system.film_height, 4, (void*)image))
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Error writing image\n");
	}
	
	delete[] image;
}
