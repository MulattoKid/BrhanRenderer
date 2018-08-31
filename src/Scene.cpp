#include "glm/geometric.hpp"
#include "Logger.h"
#include "Scene.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

bool Scene::Load(const std::string& file)
{
	//TODO: load scene file - only one obj file for now
	const int num_models = 1;
	models.resize(num_models);
	
	bool success = LoadOBJ(file, 0);
	
	//Create area lights
	for (DiffuseAreaLight& dal : diffuse_area_lights)
	{
		area_lights.push_back((AreaLight*)(&dal));
	}
	LOG_MESSAGE(true, "%lu area lights\n", area_lights.size());

	if (models.size() == 0) { LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "No models have been loaded\n"); }
	if (area_lights.size() == 0) { LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "No lights have been loaded\n"); }
	
	return success;
}

bool Scene::LoadOBJ(const std::string& file, const int model_index)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> material_ts;

	std::string err;
	bool success = tinyobj::LoadObj(&attrib, &shapes, &material_ts, &err, file.c_str()); 
	if (!err.empty()) //`err` may contain warning message
	{
	  printf("%s\n", err.c_str());
	}
	if (!success)
	{
	  return false;
	}
	if (material_ts.empty())
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "No material detected for object '%s' - nothing to render\n", file.c_str());
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
	model->materials.resize(material_ts.size());
	model->mtls.resize(material_ts.size());
	const bool has_normals = attrib.normals.size() > 0 ? true : false;
	model->has_uvs = attrib.texcoords.size() > 0 ? true : false;

	//Convert tinyobj::material:_t to MTL
	for (size_t i = 0; i < material_ts.size(); i++)
	{
		tinyobj::material_t* material = &material_ts[i];
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
		
		//TODO
		//Only matte material for now
		model->matte_materials.push_back(MatteMaterial(mtl->diffuse));
		model->materials[i] = (Material*)(&model->matte_materials[model->matte_materials.size() - 1]);
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
				if (!has_normals) //Generate normals (all are the same)
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
				tri->material = model->materials[shapes[s].mesh.material_ids[f]];
				tri->mtl = &model->mtls[shapes[s].mesh.material_ids[f]];
				//Add to lights if emissive
				if (tri->mtl->emission != glm::vec3(0.0f))
				{
					//TODO: only diffuse area light for now
					DiffuseAreaLight dal;
					dal.shape = (Shape*)(tri);
					dal.L_emit = tri->mtl->emission;
					diffuse_area_lights.push_back(dal);
					tri->area_light_index = diffuse_area_lights.size() - 1;
				}
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
				if (!has_normals) //Generate normals (all are the same)
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
				quad->material = model->materials[shapes[s].mesh.material_ids[f]];
				quad->mtl = &model->mtls[shapes[s].mesh.material_ids[f]];
				//Add to lights if emissive
				if (quad->mtl->emission != glm::vec3(0.0f))
				{
					//TODO: only diffuse area light for now
					DiffuseAreaLight dal;
					dal.shape = (Shape*)(quad);
					dal.L_emit = quad->mtl->emission;
					diffuse_area_lights.push_back(dal);
					quad->area_light_index = diffuse_area_lights.size() - 1;
				}
			}			
			index_offset += fv;
		}
	}
	
	LOG_MESSAGE(true,
				"Successfully loaded %s:\n"
				"\t%lu spheres\n"
				"\t%lu triangles\n"
				"\t%lu quads\n"
				"\t%lu shapes\n"
				"\t%lu materials\n",
				file.c_str(),
				model->spheres.size(),
				model->triangles.size(),
				model->quads.size(),
				model->shapes.size(),
				model->materials.size());
	
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
		isect->ComputeScatteringFunctions();
	}
	
	return intersected;
}

bool Scene::Intersect(Ray* ray, SurfaceInteraction* isect, const float t_less_than) const
{
	bool intersected = false;
	for (const Model& model : models)
	{
		intersected |= model.Intersect(ray, isect, t_less_than); //|= is logical OR
	}
	
	if (intersected)
	{
		isect->ray = ray;
		isect->point = ray->At();
		isect->normal = isect->shape->Normal(isect->point);
		isect->ComputeScatteringFunctions();
	}
	
	return intersected;
}


























