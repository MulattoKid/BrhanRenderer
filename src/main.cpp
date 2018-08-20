#include "Camera.h"
#include "Ray.h"
#include "Sphere.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
//#define STBI_MSC_SECURE_CRT
#include "stb/stb_image_write.h"
#include <stdio.h>
#include <vector>

int main(int argc, char** argv)
{
	Camera* camera = new Camera(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	std::vector<Sphere> spheres;
	spheres.push_back(Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));
	spheres.push_back(Sphere(glm::vec3(0.5f, 0.5f, -2.0f), 0.5f));
	spheres.push_back(Sphere(glm::vec3(-0.5f, -0.5f, -0.5f), 0.5f));

	const int width = 400, height = 200;
	unsigned char* image = new unsigned char[width * height * 4];
	const glm::vec3 top_left(-2.0f, 1.0f, -1.0f);
	const glm::vec3 horizontal(4.0f, 0.0f, 0.0f);
	const glm::vec3 vertical(0.0f, -2.0f, 0.0f);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float u = float(x) / float(width);
			float v = float(y) / float(height);
			Ray ray(camera->position, top_left + (u * horizontal) + (v * vertical));

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

			int idx = (y * width + x) * 4;
			image[idx+0] = (unsigned char)(color.x * 255.0f);
			image[idx+1] = (unsigned char)(color.y * 255.0f);
			image[idx+2] = (unsigned char)(color.z * 255.0f);
			image[idx+3] = 255;
		}
	}

	if (!stbi_write_bmp("IMAGE.bmp", width, height, 4, (void*)image))
	{
		fprintf(stderr, "Error writing image\n");
	}

	return 0;
}
