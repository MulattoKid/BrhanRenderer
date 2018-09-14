#include "Logger.h"
#include "PixelSampler.h"

PixelSampler::PixelSampler(const unsigned int spp, const unsigned int film_width, const unsigned int film_height)
{
	unsigned int i = 1;
	while (i * i <= spp) { i++; }
	i--; //Remove the last one
	
	this->spp = spp;
	this->grid_width = i;
	this->grid_height = i;
	this->pixel_width = 1.0f / film_width;
	this->pixel_height = 1.0f / film_height;
	this->remaining_samples = spp - (i * i);
	
	LOG_MESSAGE(true, "PixelSampler info:\n"
					  "\tspp %lu\n"
					  "\tgrid width %lu\n"
					  "\tgrid height %lu\n"
					  "\tpixel width %f\n"
					  "\tpixel height %f\n"
					  "\tremaining samples %lu\n",
					  this->spp, this->grid_width, this->grid_height,
					  this->pixel_width, this->pixel_height, this->remaining_samples);
}

glm::vec2 PixelSampler::Sample(const unsigned int sample_index, RNG& rng)
{
	glm::vec2 sample_offset;
	if (sample_index >= spp - remaining_samples) //Sample uniformly
	{
		float u[2];
		rng.Uniform2D(u);
		sample_offset = glm::vec2(u[0], u[1]);
	}
	else //Choose grid cell
	{
		const unsigned int y = sample_index / grid_width;
		const unsigned int x = sample_index - (grid_width * y);
		sample_offset = glm::vec2(float(x) / float(grid_width), float(y) / float(grid_height));
	}
	
	//Add an offset relative to the grid's size to make the range [0,1) into (0,1)
	static const glm::vec2 range_offset = glm::vec2(((2.0f / float(grid_width)) - (1.0f / float(grid_width))) / 2.0f);
	sample_offset += range_offset;
	
	//Convert from range (0,1) within pixel to range (0,1) within film
	sample_offset *= glm::vec2(pixel_width, pixel_height);
	
	//Move range form (0,1) within film to range (-0.5, 0.5) within film
	sample_offset -= glm::vec2(pixel_width, pixel_height) / 2.0f;
	
	return sample_offset;
}
