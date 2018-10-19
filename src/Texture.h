#ifndef TEXTURE_H
#define TEXTURE_H

#include "glm/vec4.hpp"
#include <string>

struct Texture
{
	//Base data
	int width;
	int height;
	int components_in_image;
	int components = 4;
	unsigned char* data;
	
	//Mipmaps
	unsigned int num_mipmaps;
	Texture* mipmaps;
	
	Texture(const std::string& filename);
	~Texture();
	void GenerateMipmaps();
	glm::vec4 Sample(const float u, const float v);
};

#endif
