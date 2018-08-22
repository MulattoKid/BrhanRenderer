#include "RNG.h"

void RNG::Uniform1D(float* array, int range)
{
	array[0] = GetInstance().float_distribution(GetInstance().generator);
}

void RNG::Uniform2D(float* array, int range)
{
	array[0] = GetInstance().float_distribution(GetInstance().generator);
	array[1] = GetInstance().float_distribution(GetInstance().generator);
}
