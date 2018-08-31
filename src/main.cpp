#include "Camera.h"
#include "glm/common.hpp"
#include "IntegratorUtilities.h"
#include "Ray.h"
#include "RNG.h"
#include "Scene.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
//#define STBI_MSC_SECURE_CRT
#include "stb/stb_image_write.h"
#include <stdio.h>
#include "SurfaceInteraction.h"

int main(int argc, char** argv)
{
	//Init stuff
	float u;
	RNG::Uniform1D(&u); //Get the first one out of the way....!
	
	const int width = 720, height = 480;
	const int ssp = 10;
	unsigned char* image = new unsigned char[width * height * 4];
	
	Camera* camera = new Camera(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), 70.0f, float(width) / float(height));
	Scene* scene = new Scene();
	scene->Load("models/CornellBox/CornellBox-Original.obj");
	
	for (int y = 0; y < height; y++)
	{
#pragma omp parallel for
		for (int x = 0; x < width; x++)
		{
			glm::vec3 L(0.0f);
			for (int s = 0; s < ssp; s++)
			{
				float u = float(x) / float(width);
				float v = float(y) / float(height);
				Ray ray = camera->GenerateRay(u, v);
				SurfaceInteraction isect;
				if (scene->Intersect(&ray, &isect, camera->NEAR_PLANE, camera->FAR_PLANE))
				{
					if (isect.shape->IsAreaLight())
					{
						L += scene->area_lights[isect.shape->area_light_index]->L(isect.point, -isect.ray->dir);
					}
					L += UniformSampleOne(*scene, isect);
				}
				else
				{
					float t = 0.5 * (ray.dir.y + 1.0f);
					L += (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
				}
			}
			L /= float(ssp);
			L = glm::clamp(L, 0.0f, 1.0f);

			int idx = (y * width + x) * 4;
			image[idx+0] = (unsigned char)(L.x * 255.0f);
			image[idx+1] = (unsigned char)(L.y * 255.0f);
			image[idx+2] = (unsigned char)(L.z * 255.0f);
			image[idx+3] = 255;
		}
	}

	if (!stbi_write_bmp("IMAGE.bmp", width, height, 4, (void*)image))
	{
		fprintf(stderr, "Error writing image\n");
	}

	return 0;
}
