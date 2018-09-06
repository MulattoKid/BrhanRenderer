#include "glm/common.hpp"
#include "glm/exponential.hpp"
#include "ImageIO.h"
#include "Logger.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
//#define STBI_MSC_SECURE_CRT
#include "stb/stb_image_write.h"

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

void WriteImage(const float* result, const int width, const int height)
{
	unsigned char* image = new unsigned char[width * height * 4];

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int result_idx =(y * width + x) * 3; 
			int image_idx = (y * width + x) * 4;
			for (int i = 0; i < 3; i++)
			{
				image[image_idx + i] = ToUnsignedChar(result[result_idx + i]);
			}
			image[image_idx+3] = 255;
		}
	}

	if (!stbi_write_bmp("IMAGE.bmp", width, height, 4, (void*)image))
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Error writing image\n");
	}
	
	delete[] image;
}
