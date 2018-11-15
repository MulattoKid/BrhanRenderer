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

void Render(BrhanSystem* system, Camera* camera, float* film, PixelSampler* pixel_sampler, RNG* rngs, Scene* scene);
void RenderInIntervals(BrhanSystem* system, Camera* camera, float* film, PixelSampler* pixel_sampler, RNG* rngs, Scene* scene);
void GenerateDepthImage(BrhanSystem* system, Camera* camera, float* film, Scene* scene);

int main(int argc, char** argv)
{
	auto start_time = GetTime();
	Camera* camera = NULL;
	float* film = NULL;
	float* depth_film = NULL;
	PixelSampler* pixel_sampler = NULL;
	RNG* rngs = NULL;
	Scene* scene = NULL;
	BrhanSystem system(argc, argv, &camera, &scene, &film, &depth_film, &rngs, &pixel_sampler);
	auto end_time = GetTime();
	LogElapsedTime("Intialization time: ", start_time, end_time);
	
	if (system.save_intervals_enabled)
	{
		RenderInIntervals(&system, camera, film, pixel_sampler, rngs, scene);
	}
	else
	{
		Render(&system, camera, film, pixel_sampler, rngs, scene);
	}
	
	if (system.gen_depth_image)
	{
		GenerateDepthImage(&system, camera, depth_film, scene);
	}
	
	delete scene;
	delete camera;
	delete[] depth_film;
	delete[] film;
	delete[] rngs;
	delete pixel_sampler;

	return 0;
}

void Render(BrhanSystem* system, Camera* camera, float* film, PixelSampler* pixel_sampler, RNG* rngs, Scene* scene)
{
	auto start_time = GetTime();
	//for (unsigned int y = system->film_height / 2; y < system->film_height / 2 + 1; y++)
	for (unsigned int y = 0; y < system->film_height; y++)
	{
		#pragma omp parallel for
	#ifdef _WIN32
		//for (int x = system->film_width / 2; x < system->film_width / 2 + 1; x++)
		for (int x = 0; x < int(system->film_width); x++)
	#else
		//for (unsigned int x = system->film_width / 2; x < system->film_width / 2 + 1; x++)
		for (unsigned int x = 0; x < system->film_width; x++)
	#endif
		{
			//Center coordinates of pixel
			const float u  = (float(x) + 0.5f) / float(system->film_width);
			const float ux = (float(x + 1) + 0.5f) / float(system->film_width);
			const float v  = (float(y) + 0.5f) / float(system->film_height);
			const float vy = (float(y + 1) + 0.5f) / float(system->film_height);
		
			glm::vec3 L(0.0f);
			for (unsigned int s = 0; s < system->spp; s++)
			{
				glm::vec2 sample_offset = pixel_sampler->Sample(s, rngs[omp_get_thread_num()]);
				RayDifferential ray = camera->GenerateRayDifferential(u, ux, v, vy, sample_offset);
				L += system->integrator->Li(*scene, &ray, rngs, omp_get_thread_num(), 0, system->max_depth);
			}
			
			L /= float(system->spp);
			int idx = (y * system->film_width + x) * 3;
			film[idx+0] = L.x;
			film[idx+1] = L.y;
			film[idx+2] = L.z;
		}
		
		auto update_time = GetTime();
		system->UpdateProgress(y + 1, start_time, update_time);
	}
	
	auto end_time = GetTime();
	LogElapsedTime("Render time: ", start_time, end_time);
	
	WriteImage(film, *system);
}

void RenderInIntervals(BrhanSystem* system, Camera* camera, float* film, PixelSampler* pixel_sampler, RNG* rngs, Scene* scene)
{
	unsigned int intervals[system->save_intervals + 1];
	unsigned int spp_per_interval = system->spp / system->save_intervals;
	for (unsigned int i = 0; i < system->save_intervals; i++)
	{
		intervals[i] = spp_per_interval * i;
	}
	intervals[system->save_intervals] = system->spp;
	
	glm::vec3* Ls = new glm::vec3[system->film_width * system->film_height];
	
	auto start_time = GetTime();
	for (unsigned int si = 0; si < system->save_intervals; si++)
	{
		for (unsigned int y = 0; y < system->film_height; y++)
		{
			#pragma omp parallel for
		#ifdef _WIN32
			//for (int x = system->film_width / 2; x < system->film_width / 2 + 1; x++)
			for (int x = 0; x < int(system->film_width); x++)
		#else
			//for (unsigned int x = system->film_width / 2; x < system->film_width / 2 + 1; x++)
			for (unsigned int x = 0; x < system->film_width; x++)
		#endif
			{
				//Center coordinates of pixel
				const float u  = (float(x) + 0.5f) / float(system->film_width);
				const float ux = (float(x + 1) + 0.5f) / float(system->film_width);
				const float v  = (float(y) + 0.5f) / float(system->film_height);
				const float vy = (float(y + 1) + 0.5f) / float(system->film_height);
		
				glm::vec3& L = Ls[y * system->film_width + x];
				for (unsigned int s = intervals[si]; s < intervals[si + 1]; s++)
				{
					glm::vec2 sample_offset = pixel_sampler->Sample(s, rngs[omp_get_thread_num()]);
					RayDifferential ray = camera->GenerateRayDifferential(u, ux, v, vy, sample_offset);
					L += system->integrator->Li(*scene, &ray, rngs, omp_get_thread_num(), 0, system->max_depth);
				}
					
				glm::vec3 tmp_L = L / float(intervals[si + 1]);
				int idx = (y * system->film_width + x) * 3;
				film[idx+0] = tmp_L.x;
				film[idx+1] = tmp_L.y;
				film[idx+2] = tmp_L.z;
			}
				
			auto update_time = GetTime();
			system->UpdateProgress(y + 1, start_time, update_time, si);
		}
		
		WriteImage(film, *system, si);
	}
	auto end_time = GetTime();
	LogElapsedTime("Render time: ", start_time, end_time);
	
	delete[] Ls;
}

void GenerateDepthImage(BrhanSystem* system, Camera* camera, float* film, Scene* scene)
{
	LOG_MESSAGE(true, "Generating depth image...");

	auto start_time = GetTime();
	for (unsigned int y = 0; y < system->film_height; y++)
	{
		#pragma omp parallel for
	#ifdef _WIN32
		for (int x = 0; x < int(system->film_width); x++)
	#else
		for (unsigned int x = 0; x < system->film_width; x++)
	#endif
		{
			int idx = y * system->film_width + x;
			//Center coordinates of pixel
			const float u  = (float(x) + 0.5f) / float(system->film_width);
			const float v  = (float(y) + 0.5f) / float(system->film_height);
		
			Ray ray = camera->GenerateRay(u, v);
			film[idx] = system->integrator->Depth(*scene, &ray);
		}
	}
	
	auto end_time = GetTime();
	LogElapsedTime("Depth image generation time: ", start_time, end_time);
	
	WriteDepthImage(film, *system);
}
