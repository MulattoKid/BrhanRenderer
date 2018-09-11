#include "BrhanSystem.h"
#include "Camera.h"
#include "ImageIO.h"
#include "IntegratorUtilities.h"
#include "Logger.h"
#include <omp.h>
#include "Ray.h"
#include "RNG.h"
#include "Scene.h"
#include "SurfaceInteraction.h"
#include "Timer.h"

int main(int argc, char** argv)
{
	auto start = GetTime();
	BrhanSystem system(argc, argv);
	RNG rngs[omp_get_max_threads()];
	float* image = new float[system.render_width * system.render_height * 3];
	Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), 70.0f, float(system.render_width) / float(system.render_height));
	Scene* scene = new Scene();
	scene->Load(system.render_file);
	auto end = GetTime();
	LogElapsedTime("Intialization time: ", start, end);
	
	start = GetTime();
	for (unsigned int y = 0; y < system.render_height; y++)
	{
		#pragma omp parallel for
		for (unsigned int x = 0; x < system.render_width; x++)
		{
			glm::vec3 L(0.0f);
			for (unsigned int s = 0; s < system.spp; s++)
			{
				const float u = float(x) / float(system.render_width);
				const float v = float(y) / float(system.render_height);
				Ray ray = camera->GenerateRay(u, v);
				L += system.integrator->Li(*scene, &ray, rngs[omp_get_thread_num()], 0, system.depth);
			}
			L /= float(system.spp);
			
			int idx = (y * system.render_width + x) * 3;
			image[idx+0] = L.x;
			image[idx+1] = L.y;
			image[idx+2] = L.z;
		}
		
		system.UpdateProgress(y + 1);
	}
	/*for (unsigned int y = 237; y < 238; y++)
	{
		for (unsigned int x = 354; x < 355; x++)
		{
			glm::vec3 L(0.0f);
			for (unsigned int s = 0; s < system.spp; s++)
			{
				const float u = float(x) / float(system.render_width);
				const float v = float(y) / float(system.render_height);
				Ray ray = camera->GenerateRay(u, v);
				glm::vec3 path_throughput(1.0f);
				L += system.integrator->Li(*scene, &ray, rngs[omp_get_thread_num()], 0, system.depth);
			}
			L /= float(system.spp);
			
			int idx = (y * system.render_width + x) * 3;
			image[idx+0] = L.x;
			image[idx+1] = L.y;
			image[idx+2] = L.z;
		}
	}*/
	end = GetTime();
	LogElapsedTime("Render time: ", start, end);

	WriteImage(image, system);
	
	delete scene;
	delete camera;
	delete[] image;

	return 0;
}
