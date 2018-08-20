#include <random>
#include "Random.h"

void Random1D(float* rnd)
{
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;
	rnd[0] = distribution(generator);
}

void Random2D(float* rnd)
{
	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution;
	rnd[0] = distribution(generator);
	rnd[1] = distribution(generator);
}
