#include "BrhanSystem.h"
#include "Camera.h"
#include "ImageIO.h"
#include "Logger.h"
#include "MemoryPool/MemoryPool.h"
#include <omp.h>
#include "PixelSampler.h"
#include "Ray.h"
#include "RNG.h"
#include "Scene.h"
#include "Timer.h"

int main(int argc, char** argv)
{
	auto start_time = GetTime();
	Camera* camera = NULL;
	Scene* scene = NULL;
	float* film = NULL;
	RNG* rngs = NULL;
	MemoryPool* mem_pool;
	PixelSampler* pixel_sampler = NULL;
	BrhanSystem system(argc, argv, &camera, &scene, &film, &rngs, &mem_pool, &pixel_sampler);
	auto end_time = GetTime();
	LogElapsedTime("Intialization time: ", start_time, end_time);
	
	start_time = GetTime();
	//for (unsigned int y = system.film_height / 2; y < system.film_height / 2 + 1; y++)
	for (unsigned int y = 0; y < system.film_height; y++)
	{
		#pragma omp parallel for
#ifdef _WIN32
		//for (int x = system.film_width / 2; x < system.film_width / 2 + 1; x++)
		for (int x = 0; x < int(system.film_width); x++)
#else
		//for (unsigned int x = system.film_width / 2; x < system.film_width / 2 + 1; x++)
		for (unsigned int x = 0; x < system.film_width; x++)
#endif
		{
			//Center coordinates of pixel
			const float u  = (float(x) + 0.5f) / float(system.film_width);
			const float ux = (float(x + 1) + 0.5f) / float(system.film_width);
			const float v  = (float(y) + 0.5f) / float(system.film_height);
			const float vy = (float(y + 1) + 0.5f) / float(system.film_height);
			
			glm::vec3 L(0.0f);
			for (unsigned int s = 0; s < system.spp; s++)
			{
				glm::vec2 sample_offset = pixel_sampler->Sample(s, rngs[omp_get_thread_num()]);
				RayDifferential ray = camera->GenerateRayDifferential(u, ux, v, vy, sample_offset);
				L += system.integrator->Li(*scene, &ray, rngs, mem_pool, omp_get_thread_num(), 0, system.max_depth);
			}
			L /= float(system.spp);
			
			int idx = (y * system.film_width + x) * 3;
			film[idx+0] = L.x;
			film[idx+1] = L.y;
			film[idx+2] = L.z;
		}
		auto update_time = GetTime();
		system.UpdateProgress(y + 1, start_time, update_time);
	}
	end_time = GetTime();
	LogElapsedTime("Render time: ", start_time, end_time);

	WriteImage(film, system);
	
	delete scene;
	delete camera;
	delete[] film;
	delete[] rngs;
	delete mem_pool;
	delete pixel_sampler;

	return 0;
}
