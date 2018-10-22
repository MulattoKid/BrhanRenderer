#include "Logger.h"
#include <math.h>
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
			for (int y = 0; y < mm->height; y++)
			{
				for (int x = 0; x < mm->width; x++)
				{
					//Retrieve data from previous mipmap level
					unsigned int previous_mm_width = mipmaps[i - 1].width;
					unsigned int previous_mm_height = mipmaps[i - 1].height;
					unsigned int previous_mm_x = x * (previous_mm_width / mm->width);
					unsigned int previous_mm_y = y * (previous_mm_height / mm->height);
					unsigned int previous_mm_index = (previous_mm_y * previous_mm_width + previous_mm_x) * components;
					unsigned char* previous_mm_data = mipmaps[i - 1].data;
					
					//Top left
					unsigned char tl[4] = { previous_mm_data[previous_mm_index], previous_mm_data[previous_mm_index + 1], previous_mm_data[previous_mm_index + 2], previous_mm_data[previous_mm_index + 3] };
					//Top right
					previous_mm_index += components;
					unsigned char tr[4] = { previous_mm_data[previous_mm_index], previous_mm_data[previous_mm_index + 1], previous_mm_data[previous_mm_index + 2], previous_mm_data[previous_mm_index + 3] };
					//Bottom left
					previous_mm_index += (previous_mm_width - 1) * components;
					unsigned char bl[4] = { previous_mm_data[previous_mm_index], previous_mm_data[previous_mm_index + 1], previous_mm_data[previous_mm_index + 2], previous_mm_data[previous_mm_index + 3] };
					//Bottom right
					previous_mm_index += components;
					unsigned char br[4] = { previous_mm_data[previous_mm_index], previous_mm_data[previous_mm_index + 1], previous_mm_data[previous_mm_index + 2], previous_mm_data[previous_mm_index + 3] };
					
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

glm::vec4 Texture::Sample(const float u, const float v)
{
	//TODO
	return glm::vec4();
}
