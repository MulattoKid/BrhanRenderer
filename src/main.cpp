#include "BrhanSystem.h"
#include "Camera.h"
#include "ImageIO.h"
#include "Logger.h"
#include <omp.h>
#include "PixelSampler.h"
#include "Ray.h"
#include "RNG.h"
#include "Scene.h"
#include "Timer.h"

int main(int argc, char** argv)
{
	auto start = GetTime();
	Camera* camera = NULL;
	Scene* scene = NULL;
	float* film = NULL;
	RNG* rngs = NULL;
	PixelSampler* pixel_sampler = NULL;
	BrhanSystem system(argc, argv, &camera, &scene, &film, &rngs, &pixel_sampler);
	auto end = GetTime();
	LogElapsedTime("Intialization time: ", start, end);
	
	start = GetTime();
	for (unsigned int y = 0; y < system.film_height; y++)
	{
		#pragma omp parallel for
		for (unsigned int x = 0; x < system.film_width; x++)
		{
			//Center coordinates of pixel
			const float u = (float(x) + 0.5f) / float(system.film_width);
			const float v = (float(y) + 0.5f) / float(system.film_height);
			
			glm::vec3 L(0.0f);
			for (unsigned int s = 0; s < system.spp; s++)
			{
				glm::vec2 sample_offset = pixel_sampler->Sample(s, rngs[omp_get_thread_num()]);
				Ray ray = camera->GenerateRay(u + sample_offset.x, v + sample_offset.y);
				L += system.integrator->Li(*scene, &ray, rngs[omp_get_thread_num()], 0, system.max_depth);
			}
			L /= float(system.spp);
			
			int idx = (y * system.film_width + x) * 3;
			film[idx+0] = L.x;
			film[idx+1] = L.y;
			film[idx+2] = L.z;
		}
		
		system.UpdateProgress(y + 1);
	}
	end = GetTime();
	LogElapsedTime("Render time: ", start, end);

	WriteImage(film, system);
	
	delete scene;
	delete camera;
	delete[] film;
	delete[] rngs;
	delete pixel_sampler;

	return 0;
}
