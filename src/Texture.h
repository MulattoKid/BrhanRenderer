#ifndef TEXTURE_H
#define TEXTURE_H

#include "glm/vec4.hpp"
#include <string>

struct Mipmap
{
	int width;
	int height;
	int components = 4;
	int mipmap_level;
	unsigned char* data;
	
	~Mipmap();
};

struct Texture
{
	//Base data
	std::string file;
	int width;
	int height;
	int components_in_image;
	int components = 4;
	
	//Mipmaps
	unsigned int num_mipmaps;
	Mipmap* mipmaps;
	
	Texture(const std::string& filename);
	~Texture();
	void GenerateMipmaps(const unsigned char* data);
	glm::vec3 Sample(float u, float v, float dudx, float dvdx, float dudy, float dvdy);
};

#endif
