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
	//Init stuff
	RNG rngs[omp_get_max_threads()];
	const int width = 720, height = 480;
	const int ssp = 20;
	float* image = new float[width * height * 3];

	auto start = GetTime();
	Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), 70.0f, float(width) / float(height));
	Scene* scene = new Scene();
	scene->Load("models/CornellBox/CornellBox-Original.obj");
	auto end = GetTime();
	LogElapsedTime("Load time: ", start, end);
	
	start = GetTime();
	for (int y = 0; y < height; y++)
	{
//#pragma omp parallel for
		for (int x = 0; x < width; x++)
		{
			float u = float(x) / float(width);
			float v = float(y) / float(height);
			Ray ray = camera->GenerateRay(u, v);
			SurfaceInteraction isect;
			
			glm::vec3 L(0.0f);
			if (scene->Intersect(&ray, &isect, camera->NEAR_PLANE, camera->FAR_PLANE))
			{
				for (int s = 0; s < ssp; s++)
				{
					if (isect.shape->IsAreaLight())
					{
						L += scene->area_lights[isect.shape->area_light_index]->L(isect.point, -isect.ray->dir);
					}
					else //TODO: rmeove when done debugging
					{
						L += UniformSampleOne(*scene, isect, rngs[omp_get_thread_num()]);
						if (L == glm::vec3(0.0f))
						{
							//Problem: a pixel will get either all OK or OK2, which results in some pixels being completely black
							//Why does this happen??
							//LOG_MESSAGE(false, "OK\n");
						}
						else
						{
							//LOG_MESSAGE(false, "OK2\n");
						}
					}
				}
				//LOG_MESSAGE(true, "--------------\n");
			}
			L /= float(ssp);
			
			int idx = (y * width + x) * 3;
			image[idx+0] = L.x;
			image[idx+1] = L.y;
			image[idx+2] = L.z;
		}
	}
	end = GetTime();
	LogElapsedTime("Render time: ", start, end);

	WriteImage(image, width, height);
	
	delete scene;
	delete camera;
	delete[] image;

	return 0;
}
