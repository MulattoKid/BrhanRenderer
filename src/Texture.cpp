#include "Logger.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "Texture.h"

Texture::Texture(const std::string& filename)
{
	data = stbi_load(filename.c_str(), &width, &height, &components_in_image, components);
	if (data == NULL)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to load image %s\n", filename.c_str());
	}
	
	GenerateMipmaps();
}

Texture::~Texture()
{
	stbi_image_free(data);
}

void Texture::GenerateMipmaps()
{
	//TODO
}

glm::vec4 Texture::Sample(const float u, const float v)
{
	//TODO
	return glm::vec4();
}
