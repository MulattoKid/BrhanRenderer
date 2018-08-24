#ifndef RNG_H
#define RNG_H

#include <random>
#include <time.h>

struct RNG
{
public:
	static float Uniform1D();
	static void Uniform1D(float* array);
	static void Uniform2D(float* array);

	RNG(RNG const&) = delete;
	void operator=(RNG const&) = delete;
private:
	RNG()
	{
		generator = std::default_random_engine((unsigned int)(time(NULL)));
		float_distribution = std::uniform_real_distribution<float>(0.0f, 1.0f);
	};
	static RNG& GetInstance()
	{
		static RNG instance;
		return instance;
	}

	std::default_random_engine generator;
	std::uniform_real_distribution<float> float_distribution;
};

#endif
