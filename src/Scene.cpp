#include "glm/geometric.hpp"
#include "Scene.h"
#include <stdio.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

bool Scene::Load(const std::string& file)
{
	//TODO: load scene file - only one obj file for now
	const int num_models = 1;
	models.resize(num_models);
	
	bool success = LoadOBJ(file, 0);
	
	return success;
}

bool Scene::LoadOBJ(const std::string& file, const int model_index)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file.c_str()); 
	if (!err.empty()) //`err` may contain warning message
	{
	  printf("%s\n", err.c_str());
	}
	if (!success)
	{
	  return false;
	}
	
	//Count number of triangles and quads to resize Model::shapes
	Model* model = &models[model_index];
	int num_triangles = 0, num_quads = 0;
	for (size_t s = 0; s < shapes.size(); s++)
	{
		//Loop over faces(polygon)
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			int fv = shapes[s].mesh.num_face_vertices[f];
			if (fv == 3) { num_triangles++; }
			else { num_quads++; }
		}
	}
	model->triangles.resize(num_triangles);
	model->quads.resize(num_quads);
	model->shapes.resize(num_triangles + num_quads);
	model->mtls.resize(materials.size());
	const bool has_normals = attrib.normals.size() > 0 ? true : false;
	model->has_uvs = attrib.texcoords.size() > 0 ? true : false;

	//Convert tinyobj::material:_t to MTL
	for (size_t i = 0; i < materials.size(); i++)
	{
		tinyobj::material_t* material = &materials[i];
		MTL* mtl = &model->mtls[i];

		//Assign
		mtl->ambient = glm::vec3(material->ambient[0], material->ambient[1], material->ambient[2]);
		mtl->diffuse = glm::vec3(material->diffuse[0], material->diffuse[1], material->diffuse[2]);
		mtl->specular = glm::vec3(material->specular[0], material->specular[1], material->specular[2]);
		mtl->transmittance = glm::vec3(material->transmittance[0], material->transmittance[1], material->transmittance[2]);
		mtl->emission = glm::vec3(material->emission[0], material->emission[1], material->emission[2]);
		mtl->shininess = material->shininess;
		mtl->index_of_refraction = material->ior;
		mtl->dissolve = material->dissolve;
		mtl->illumination_model = material->illum;
	}

	//Loop over shapes - remember that what I call a Shape is NOT the same as the OBJ view of a shape
	int triangle_index = 0, quad_index = 0, shape_index = 0;
	for (size_t s = 0; s < shapes.size(); s++)
	{
		//Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			size_t fv = shapes[s].mesh.num_face_vertices[f];
			if (fv == 3) //Triangle
			{
				Triangle* tri = &model->triangles[triangle_index++];
				//Loop over vertices in the face
				for (size_t v = 0; v < fv; v++)
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					tri->v[v].x = attrib.vertices[3*idx.vertex_index+0];
					tri->v[v].y = attrib.vertices[3*idx.vertex_index+1];
				  	tri->v[v].z = attrib.vertices[3*idx.vertex_index+2];

					if (has_normals)
					{
						tri->n[v].x = attrib.normals[3*idx.normal_index+0];
					  	tri->n[v].y = attrib.normals[3*idx.normal_index+1];
					  	tri->n[v].z = attrib.normals[3*idx.normal_index+2];
					}

					if (model->has_uvs)
					{
					  	tri->uv[v].x = attrib.texcoords[3*idx.texcoord_index+0];
					  	tri->uv[v].y = attrib.texcoords[3*idx.texcoord_index+1];
					}
				}
				if (!has_normals) //Generate normals - all are the same
				{
					glm::vec3 v0v1 = tri->v[1] - tri->v[0];
					glm::vec3 v0v2 = tri->v[2] - tri->v[0];
					glm::vec3 normal = glm::normalize(glm::cross(v0v1, v0v2));
					tri->n[0] = normal;
					tri->n[1] = normal;
					tri->n[2] = normal;
				}
				model->shapes[shape_index++] = tri;
				
				//per-face material
				tri->mtl = &model->mtls[shapes[s].mesh.material_ids[f]];
			}
			else //Quad
			{
				Quad* quad = &model->quads[quad_index++];
				//Loop over vertices in the face
				for (size_t v = 0; v < fv; v++)
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				  	quad->v[v].x = attrib.vertices[3*idx.vertex_index+0];
				  	quad->v[v].y = attrib.vertices[3*idx.vertex_index+1];
				  	quad->v[v].z = attrib.vertices[3*idx.vertex_index+2];

					if (has_normals)
					{
					  	quad->n[v].x = attrib.normals[3*idx.normal_index+0];
					  	quad->n[v].y = attrib.normals[3*idx.normal_index+1];
					  	quad->n[v].z = attrib.normals[3*idx.normal_index+2];
					}

					if (model->has_uvs)
					{
					  	quad->uv[v].x = attrib.texcoords[3*idx.texcoord_index+0];
					  	quad->uv[v].y = attrib.texcoords[3*idx.texcoord_index+1];
					}
				}
				if (!has_normals) //Generate normals - all are the same
				{
					glm::vec3 v0v1 = quad->v[1] - quad->v[0];
					glm::vec3 v0v2 = quad->v[2] - quad->v[0];
					glm::vec3 normal = glm::normalize(glm::cross(v0v1, v0v2));
					quad->n[0] = normal;
					quad->n[1] = normal;
					quad->n[2] = normal;
					quad->n[3] = normal;
				}
				model->shapes[shape_index++] = quad;
				
				//per-face material
				quad->mtl = &model->mtls[shapes[s].mesh.material_ids[f]];
			}			
			index_offset += fv;
		}
	}
	
	printf("Successfully loaded %s:\n", file.c_str());
	printf("\t%lu spheres\n", model->spheres.size());
	printf("\t%lu triangles\n", model->triangles.size());
	printf("\t%lu quads\n", model->quads.size());
	printf("\t%lu shapes\n", model->shapes.size());
	printf("\t%lu materials\n", model->mtls.size());
	
	return true;
}

bool Scene::Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const
{
	bool intersected = false;
	for (const Model& model : models)
	{
		intersected |= model.Intersect(ray, isect, t_min, t_max); //|= is logical OR
	}
	
	if (intersected)
	{
		isect->ray = ray;
		isect->point = ray->At();
		isect->normal = isect->shape->Normal(isect->point);
	}
	
	return intersected;
}




























