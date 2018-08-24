#include "RNG.h"

float RNG::Uniform1D()
{
	return GetInstance().float_distribution(GetInstance().generator);
}

void RNG::Uniform1D(float* array)
{
	array[0] = GetInstance().float_distribution(GetInstance().generator);
}

void RNG::Uniform2D(float* array)
{
	array[0] = GetInstance().float_distribution(GetInstance().generator);
	array[1] = GetInstance().float_distribution(GetInstance().generator);
}
