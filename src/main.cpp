#include "Camera.h"
#include "Ray.h"
#include "RNG.h"
#include "Sphere.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
//#define STBI_MSC_SECURE_CRT
#include "stb/stb_image_write.h"
#include <stdio.h>
#include <vector>

int main(int argc, char** argv)
{
	const int width = 480, height = 270;
	const int ssp = 100;
	unsigned char* image = new unsigned char[width * height * 4];
	
	Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 70.0f, float(width) / float(height));
	std::vector<Sphere> spheres;
	spheres.push_back(Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));
	spheres.push_back(Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f));
	
	for (int y = 0; y < height; y++)
	{
#pragma omp parallel for
		for (int x = 0; x < width; x++)
		{
			glm::vec3 acc_color(0.0f);
			for (int s = 0; s < ssp; s++)
			{
				float rnd[2];
				RNG::Uniform2D(rnd);
				float u = (float(x) + rnd[0]) / float(width);
				float v = (float(y) + rnd[1]) / float(height);
				Ray ray = camera->GenerateRay(u, v);

				float t = 0.5 * (ray.dir.y + 1.0f);
				glm::vec3 color = (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
			
				for (Sphere& sphere : spheres)
				{
					if (sphere.Intersect(&ray, camera->NEAR_PLANE, camera->FAR_PLANE))
					{
						color = sphere.Normal(ray.At());
						color += 1.0f;
						color *= 0.5f;
					}
				}
				acc_color += color;
			}
			acc_color /= float(ssp);

			int idx = (y * width + x) * 4;
			image[idx+0] = (unsigned char)(acc_color.x * 255.0f);
			image[idx+1] = (unsigned char)(acc_color.y * 255.0f);
			image[idx+2] = (unsigned char)(acc_color.z * 255.0f);
			image[idx+3] = 255;
		}
	}

	if (!stbi_write_bmp("IMAGE.bmp", width, height, 4, (void*)image))
	{
		fprintf(stderr, "Error writing image\n");
	}

	return 0;
}
