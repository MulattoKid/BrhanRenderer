#ifndef PIXELSAMPLER_H
#define PIXELSAMPLER_H

#include "glm/vec2.hpp"
#include "RNG.h"

struct PixelSampler
{
	unsigned int spp;
	unsigned int grid_width;
	unsigned int grid_height;
	float pixel_width;
	float pixel_height;
	unsigned int remaining_samples;

	PixelSampler(const unsigned int spp, const unsigned int film_width, const unsigned int film_height);
	glm::vec2 Sample(const unsigned int sample_index, RNG& rng);
};

#endif
