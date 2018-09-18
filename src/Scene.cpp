#include "BrhanSystem.h"
#include "glm/geometric.hpp"
#include "Logger.h"
#include "Scene.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

Scene::Scene(const std::vector<std::string>& model_files, const std::vector<SphereLoad>& spheres)
{	
	models.resize(model_files.size() + spheres.size());

	//Load obj files
	for (const std::string& file : model_files)
	{
		if (!LoadOBJ(file.c_str(), 0))
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to load model %s\n", file.c_str());
		}
	}
	
	//Load spheres
	unsigned int sphere_index = 0;
	for (const SphereLoad& sphere : spheres)
	{
		if (!LoadSphere(sphere, model_files.size() + sphere_index))
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to load sphere number %lu\n", sphere_index);
		}
		sphere_index++;
	}
	
	//Create area lights
	for (DiffuseAreaLight& dal : diffuse_area_lights)
	{
		area_lights.push_back((AreaLight*)(&dal));
	}
	LOG_MESSAGE(true, "%lu area lights\n", area_lights.size());

	if (models.size() == 0) { LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "No models have been loaded\n"); }
	if (area_lights.size() == 0) { LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "No lights have been loaded\n"); }
}

bool Scene::LoadOBJ(const char* file, const int model_index)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> material_ts;

	std::string err;
	bool success = tinyobj::LoadObj(&attrib, &shapes, &material_ts, &err, file); 
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
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "No material detected for object '%s' - nothing to render\n", file);
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
	std::vector<int> matte_material_indices;
	std::vector<int> mirror_material_indices;
	std::vector<int> plastic_material_indices;
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
		
		//TODO: cover all supported materials
		if (mtl->diffuse != glm::vec3(0.0f) && mtl->specular == glm::vec3(0.0f))
		{
			model->matte_materials.push_back(MatteMaterial(mtl->diffuse));
			matte_material_indices.push_back(i);
		}
		else if (mtl->diffuse == glm::vec3(0.0f) && mtl->specular != glm::vec3(0.0f))
		{
			model->mirror_materials.push_back(MirrorMaterial(mtl->specular));
			mirror_material_indices.push_back(i);
		}
		else if (mtl->diffuse != glm::vec3(0.0f) && mtl->specular != glm::vec3(0.0f))
		{
			model->plastic_materials.push_back(PlasticMaterial(mtl->diffuse, mtl->specular));
			plastic_material_indices.push_back(i);
		}
		else
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Unsupported material\n");
		}
	}
	
	//TODO: cover all supported materials
	//Copy over pointers to model->materials
	for (size_t i = 0; i < model->matte_materials.size(); i++)
	{
		model->materials[matte_material_indices[i]] = (Material*)(&model->matte_materials[i]);
	}
	for (size_t i = 0; i < model->mirror_materials.size(); i++)
	{
		model->materials[mirror_material_indices[i]] = (Material*)(&model->mirror_materials[i]);
	}
	for (size_t i = 0; i < model->plastic_materials.size(); i++)
	{
		model->materials[plastic_material_indices[i]] = (Material*)(&model->plastic_materials[i]);
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
				"\t%lu materials\n"
				"\t\t%lu matte\n"
				"\t\t%lu mirror\n"
				"\t\t%lu plastic\n",
				file,
				model->spheres.size(),
				model->triangles.size(),
				model->quads.size(),
				model->shapes.size(),
				model->materials.size(),
				model->matte_materials.size(),
				model->mirror_materials.size(),
				model->plastic_materials.size());
	
	return true;
}

bool Scene::LoadSphere(const SphereLoad& sphere, const int model_index)
{
	Model* model = &models[model_index];
	model->spheres.push_back(Sphere(sphere.center, sphere.radius));
	
	//TODO: cover all supported materials
	if (sphere.material == "matte")
	{
		model->matte_materials.push_back(MatteMaterial(sphere.diffuse));
		model->materials.push_back(&model->matte_materials[0]);
	}
	else if (sphere.material == "mirror")
	{
		model->mirror_materials.push_back(MirrorMaterial(sphere.specular));
		model->materials.push_back(&model->mirror_materials[0]);
	}
	else if (sphere.material == "plastic")
	{
		model->plastic_materials.push_back(PlasticMaterial(sphere.diffuse, sphere.specular));
		model->materials.push_back(&model->plastic_materials[0]);
	}
	else if (sphere.material == "translucent")
	{
		model->translucent_materials.push_back(TranslucentMaterial(sphere.specular, 1.0f, 1.0f));
		model->materials.push_back(&model->translucent_materials[0]);
	}
	else
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Material %s is not supported\n", sphere.material);
	}
	
	model->spheres[0].material = model->materials[0];
	model->shapes.push_back(&model->spheres[0]);
	
	LOG_MESSAGE(true,
				"Successfully loaded sphere:\n"
				"\t[%f %f %f] center\n"
				"\t%f radius\n"
				"\t%s material\n"
				"\t\t[%f %f %f] Kd\n"
				"\t\t[%f %f %f] Ks\n",
				model->spheres[0].center.x, model->spheres[0].center.y, model->spheres[0].center.z,
				model->spheres[0].radius,
				sphere.material.c_str(),
				sphere.diffuse.x, sphere.diffuse.y, sphere.diffuse.z,
				sphere.specular.x, sphere.specular.y, sphere.specular.z);
				
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
		isect->wo = -ray->dir;
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
		isect->wo = -ray->dir;
		isect->ComputeScatteringFunctions();
	}
	
	return intersected;
}


























