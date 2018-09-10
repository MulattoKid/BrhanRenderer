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
			float u = float(x) / float(system.render_width);
			float v = float(y) / float(system.render_height);
			Ray ray = camera->GenerateRay(u, v);
			SurfaceInteraction isect;
			
			glm::vec3 L(0.0f);
			if (scene->Intersect(&ray, &isect, camera->NEAR_PLANE, camera->FAR_PLANE))
			{
				for (unsigned int s = 0; s < system.spp; s++)
				{
					L += system.integrator->Li(*scene, isect, rngs[omp_get_thread_num()]);
				}
				L /= float(system.spp);
			}
			
			int idx = (y * system.render_width + x) * 3;
			image[idx+0] = L.x;
			image[idx+1] = L.y;
			image[idx+2] = L.z;
		}
		
		system.UpdateProgress(y + 1);
	}
	end = GetTime();
	LogElapsedTime("\nRender time: ", start, end);

	WriteImage(image, system.render_width, system.render_height);
	
	delete scene;
	delete camera;
	delete[] image;

	return 0;
}
