#include "glm/common.hpp"
#include "Logger.h"
#include <math.h>
#include <omp.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "Texture.h"

Mipmap::~Mipmap()
{
	delete[] data;
}

Texture::Texture(const std::string& filename)
{
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &components_in_image, components);
	if (data == NULL)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to load image %s\n", filename.c_str());
	}
	
	GenerateMipmaps(data);
	stbi_image_free(data);
}

Texture::~Texture()
{
	delete[] mipmaps;
}

void Texture::GenerateMipmaps(const unsigned char* data)
{
	num_mipmaps = floor(log2(std::max(width, height))) + 1;
	mipmaps = new Mipmap[num_mipmaps];
	
	int mm_width = width;
	int mm_height = height;
	for (unsigned int i = 0; i < num_mipmaps; i++)
	{
		Mipmap* mm = &mipmaps[i];
		mm->mipmap_level = i;
		mm->width = mm_width;
		mm->height = mm_height;
		mm->components = components;
		mm->data = new unsigned char[mm->width * mm->height * mm->components];
		
		if (i == 0)
		{
			memcpy(mm->data, data, width * height * components);
		}
		else
		{
			//Retrieve data from previous mipmap level
			const unsigned int previous_mm_width = mipmaps[i - 1].width;
			const unsigned int previous_mm_height = mipmaps[i - 1].height;
			const unsigned int mm_to_previous_mm_scale_x = previous_mm_width / mm->width;
			const unsigned int mm_to_previous_mm_scale_y = previous_mm_height / mm->height;
			for (int y = 0; y < mm->height; y++)
			{
				#pragma omp parallel for
				for (int x = 0; x < mm->width; x++)
				{
					//Retrieve data from previous mipmap level
					unsigned int previous_mm_x = x * mm_to_previous_mm_scale_x;
					unsigned int previous_mm_y = y * mm_to_previous_mm_scale_y;
					unsigned int previous_mm_index = (previous_mm_y * previous_mm_width + previous_mm_x) * components;
					unsigned char* previous_mm_data = mipmaps[i - 1].data;
					
					//Note: using unsigned short instead of unsigned char as I don't want
					//values to wrap around the 255 limit when adding and averaging below
					//Top left
					unsigned short tl[4] = { previous_mm_data[previous_mm_index], previous_mm_data[previous_mm_index + 1], previous_mm_data[previous_mm_index + 2], previous_mm_data[previous_mm_index + 3] };
					//Top right
					previous_mm_index += components;
					unsigned short tr[4] = { previous_mm_data[previous_mm_index], previous_mm_data[previous_mm_index + 1], previous_mm_data[previous_mm_index + 2], previous_mm_data[previous_mm_index + 3] };
					//Bottom left
					previous_mm_index += (previous_mm_width - 1) * components;
					unsigned short bl[4] = { previous_mm_data[previous_mm_index], previous_mm_data[previous_mm_index + 1], previous_mm_data[previous_mm_index + 2], previous_mm_data[previous_mm_index + 3] };
					//Bottom right
					previous_mm_index += components;
					unsigned short br[4] = { previous_mm_data[previous_mm_index], previous_mm_data[previous_mm_index + 1], previous_mm_data[previous_mm_index + 2], previous_mm_data[previous_mm_index + 3] };
					
					//Average and assign data to new mipmap
					unsigned int mm_index = (y * mm->width + x) * components;
					mm->data[mm_index + 0] = (tl[0] + tr[0] + bl[0] + br[0]) / 4;
					mm->data[mm_index + 1] = (tl[1] + tr[1] + bl[1] + br[1]) / 4;
					mm->data[mm_index + 2] = (tl[2] + tr[2] + bl[2] + br[2]) / 4;
					mm->data[mm_index + 3] = (tl[3] + tr[3] + bl[3] + br[3]) / 4;
				}
			}
		}
		
		mm_width /= 2;
		mm_height /= 2;
	}
}

glm::vec4 Texture::Sample(float u, float v)
{
	//TODO: select mipmap level
	const unsigned int mipmap_level = 0;
	Mipmap* mm = &mipmaps[mipmap_level];
	
	//Find uv coefficients
	u = u * mm->width - 0.5f;
	v = v * mm->height - 0.5f;
	const int x = glm::floor(u);
	const int y = glm::floor(v);
	float u_ratio = u - x;
	float v_ratio = v - y;
	float u_opposite = 1.0f - u_ratio;
	float v_opposite = 1.0f - v_ratio;
	
	//Bilinear filtering - CLAMP
	const unsigned int mm_index = (y * mm->width + x) * mm->components;
	const unsigned int tl_index = mm_index;
	unsigned int tr_index = 0, bl_index = 0, br_index = 0;
	if (x < mm->width) { tr_index = mm_index + mm->components; }
	else { tr_index = mm_index; }
	if (y < mm->height)
	{
		bl_index = mm_index + (mm->width * mm->components);
		if (x < mm->width)
		{
			br_index = mm_index + ((mm->width + 1) * mm->components);
		}
		else
		{
			br_index = mm_index + (mm->width * mm->components);
		}
	}
	else
	{
		bl_index = mm_index;
		if (x < mm->width)
		{
			br_index = mm_index + mm->components;
		}
		else
		{
			br_index = mm_index;
		}
	}
	
	glm::vec4 ret(0.0f);
	for (int i = 0; i < mm->components; i++)
	{
		ret[i] = (mm->data[tl_index] * u_opposite + mm->data[tr_index] * u_ratio) * v_opposite +
				 (mm->data[bl_index] * u_opposite + mm->data[br_index] * u_ratio) * v_ratio;
	}
	return ret / 255.0f;
}






































