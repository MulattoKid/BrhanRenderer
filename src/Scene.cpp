#include "BrhanSystem.h"
#include "glm/geometric.hpp"
#include "Logger.h"
#include "Scene.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

Scene::Scene(const std::vector<ModelLoad>& models, const std::vector<SphereLoad>& spheres)
{	
	this->models.resize(models.size() + spheres.size());

	//Load obj files
	unsigned int model_index = 0;
	for (const ModelLoad& model : models)
	{
		if (!LoadOBJ(model, model_index))
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Failed to load model %s\n", model.file.c_str());
		}
		model_index++;
	}
	
	//Load spheres
	unsigned int sphere_index = 0;
	for (const SphereLoad& sphere : spheres)
	{
		if (!LoadSphere(sphere, models.size() + sphere_index))
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

	if (this->models.size() == 0) { LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "No models have been loaded\n"); }
	if (area_lights.size() == 0) { LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "No lights have been loaded\n"); }
	
	//Build BVH tree
	unsigned int num_shapes = 0;
	for (Model& m : this->models)
	{
		num_shapes += m.shapes.size();
	}
	std::vector<Shape*> shapes(num_shapes);
	unsigned int shape_index = 0;
	for (Model& m : this->models)
	{
		for (Shape* s : m.shapes)
		{
			shapes[shape_index++] = s;
		}
	}
	bvh_tree = new BVH(1, shapes);
	
	LOG_MESSAGE(true, "Scene:\n"
					  "\t%lu models\n"
					  "\t%lu area lights\n",
					  this->models.size(),
					  area_lights.size());
}

bool Scene::LoadOBJ(const ModelLoad& model_load, const unsigned int model_index)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> material_ts;

	std::string err;
	bool success = tinyobj::LoadObj(&attrib, &shapes, &material_ts, &err, model_load.file.c_str()); 
	if (!err.empty()) //`err` may contain warning message
	{
	  printf("%s\n", err.c_str());
	}
	if (!success)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "tinyobjloader failed\n");
	}
	if (material_ts.empty() && !model_load.has_custom_material)
	{
		LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "No material detected for model '%s' - nothing to render\n", model_load.file.c_str());
	}
	
	//Count number of triangles and quads to resize Model::shapes
	Model* model = &models[model_index];
	int num_triangles = 0;
	for (size_t s = 0; s < shapes.size(); s++)
	{
		//Loop over faces(polygon)
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			int fv = shapes[s].mesh.num_face_vertices[f];
			if (fv == 3) { num_triangles++; }
			else
			{
				LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Unsupported number of vertices (%i) for shape %lu in model '%s'", fv, s, model_load.file.c_str());
			}
		}
	}
	model->triangles.resize(num_triangles);
	model->shapes.resize(num_triangles);
	if (model_load.has_custom_material)
	{
		model->materials.resize(1);
		model->mtls.resize(1);
	}
	else
	{
		model->materials.resize(material_ts.size());
		model->mtls.resize(material_ts.size());
	}
	const bool has_normals = attrib.normals.size() > 0 ? true : false;
	model->has_uvs = attrib.texcoords.size() > 0 ? true : false;

	//Convert tinyobj::material:_t to MTL
	std::vector<int> matte_material_indices;
	std::vector<int> mirror_material_indices;
	std::vector<int> plastic_material_indices;
	std::vector<int> translucent_material_indices;
	std::vector<int> water_material_indices;
	std::vector<int> glass_material_indices;
	if (model_load.has_custom_material)
	{
		//TODO: cover all supported materials
		if (model_load.material == "matte")
		{
			model->matte_materials.push_back(MatteMaterial(model_load.diffuse));
			matte_material_indices.push_back(0);
		}
		else if (model_load.material == "mirror")
		{
			model->mirror_materials.push_back(MirrorMaterial(model_load.specular));
			mirror_material_indices.push_back(0);
		}
		else if (model_load.material == "plastic")
		{
			model->plastic_materials.push_back(PlasticMaterial(model_load.diffuse, model_load.specular));
			plastic_material_indices.push_back(0);
		}
		else if (model_load.material == "translucent")
		{
			model->translucent_materials.push_back(TranslucentMaterial(model_load.transmittance));
			translucent_material_indices.push_back(0);
		}
		else if (model_load.material == "water")
		{
			model->water_materials.push_back(WaterMaterial(model_load.reflectance, model_load.transmittance));
			water_material_indices.push_back(0);
		}
		else if (model_load.material == "glass")
		{
			model->glass_materials.push_back(GlassMaterial(model_load.reflectance, model_load.transmittance));
			glass_material_indices.push_back(0);
		}
		else
		{
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Material %s is not supported\n", model_load.material);
		}
		
		//TODO: update as more support is added for custom materials
		model->mtls[0].ambient = glm::vec3(0.0f);
		model->mtls[0].diffuse = model_load.diffuse;
		model->mtls[0].specular = model_load.specular;
		model->mtls[0].transmittance = model_load.transmittance;
		model->mtls[0].emission = glm::vec3(0.0f);
		model->mtls[0].shininess = 1.0f;
		model->mtls[0].index_of_refraction = 1.0f;
		model->mtls[0].dissolve = 1.0f;
		model->mtls[0].illumination_model = 2;
	}
	else
	{
		for (size_t i = 0; i < material_ts.size(); i++)
		{
			tinyobj::material_t* material = &material_ts[i];
			MTL* mtl = &model->mtls[i];

			//Assign
			mtl->ambient = glm::vec3(material->ambient[0], material->ambient[1], material->ambient[2]);
			mtl->map_ka = material->ambient_texname;
			mtl->diffuse = glm::vec3(material->diffuse[0], material->diffuse[1], material->diffuse[2]);
			mtl->map_kd = material->diffuse_texname;
			mtl->specular = glm::vec3(material->specular[0], material->specular[1], material->specular[2]);
			mtl->map_ks = material->specular_texname;
			mtl->transmittance = glm::vec3(material->transmittance[0], material->transmittance[1], material->transmittance[2]);
			mtl->emission = glm::vec3(material->emission[0], material->emission[1], material->emission[2]);
			mtl->shininess = material->shininess;
			mtl->index_of_refraction = material->ior;
			mtl->dissolve = material->dissolve;
			mtl->illumination_model = material->illum;
		
			//TODO: cover all supported materials
			if (mtl->dissolve == 1.0f) //Fully opaque materials
			{
				if ((mtl->diffuse != glm::vec3(0.0f) && mtl->specular == glm::vec3(0.0f)) ||
					(mtl->map_kd != "" && mtl->map_ks == ""))
				{
					if (mtl->map_kd != "")
					{
						model->matte_materials.push_back(MatteMaterial(mtl->map_kd));
					}
					else
					{
						model->matte_materials.push_back(MatteMaterial(mtl->diffuse));
					}
					matte_material_indices.push_back(i);
				}
				else if ((mtl->diffuse == glm::vec3(0.0f) && mtl->specular != glm::vec3(0.0f)) ||
						 (mtl->map_kd == "" && mtl->map_ks != ""))
				{
					if (mtl->map_ks != "")
					{
						model->mirror_materials.push_back(MirrorMaterial(mtl->map_ks));
					}
					else
					{
						model->mirror_materials.push_back(MirrorMaterial(mtl->specular));
					}
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
			else //Some transparency materials
			{
				if (mtl->index_of_refraction == 1.0f)
				{
					model->translucent_materials.push_back(TranslucentMaterial(mtl->transmittance));
					translucent_material_indices.push_back(i);
				}
				else if (mtl->index_of_refraction >= 1.32f && mtl->index_of_refraction <= 1.35f)
				{
					model->water_materials.push_back(WaterMaterial(mtl->specular, mtl->transmittance));
					water_material_indices.push_back(i);
				}
				else if (mtl->index_of_refraction >= 1.5f && mtl->index_of_refraction <= 1.6f)
				{
					model->glass_materials.push_back(GlassMaterial(mtl->specular, mtl->transmittance));
					glass_material_indices.push_back(i);
				}
				else
				{
					LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "Unsupported material\n");
				}
			}
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
	for (size_t i = 0; i < model->translucent_materials.size(); i++)
	{
		model->materials[translucent_material_indices[i]] = (Material*)(&model->translucent_materials[i]);
	}
	for (size_t i = 0; i < model->water_materials.size(); i++)
	{
		model->materials[water_material_indices[i]] = (Material*)(&model->water_materials[i]);
	}
	for (size_t i = 0; i < model->glass_materials.size(); i++)
	{
		model->materials[glass_material_indices[i]] = (Material*)(&model->glass_materials[i]);
	}

	//Loop over shapes - remember that what I call a Shape is NOT the same as the OBJ view of a shape
	int triangle_index = 0;
	for (size_t s = 0; s < shapes.size(); s++)
	{
		//Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			size_t fv = shapes[s].mesh.num_face_vertices[f];
			if (fv == 3) //Triangle
			{
				Triangle* tri = &model->triangles[triangle_index];
				//Loop over vertices in the face
				for (size_t v = 0; v < fv; v++)
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					tri->v[v].x = attrib.vertices[3*idx.vertex_index+0];
					tri->v[v].y = attrib.vertices[3*idx.vertex_index+1];
				  	tri->v[v].z = attrib.vertices[3*idx.vertex_index+2];
				  	if (model_load.scaling_active)
				  	{
				  		tri->v[v] = glm::vec3(model_load.scaling * glm::vec4(tri->v[v], 1.0f));
				  	}
				  	if (model_load.rotation_active)
				  	{
				  		tri->v[v] = glm::vec3(model_load.rotation * glm::vec4(tri->v[v], 1.0f));
				  	}
				  	if (model_load.translation_active)
				  	{
				  		tri->v[v] = glm::vec3(model_load.translation * glm::vec4(tri->v[v], 1.0f));
				  	}

					if (has_normals)
					{
						tri->n[v].x = attrib.normals[3*idx.normal_index+0];
					  	tri->n[v].y = attrib.normals[3*idx.normal_index+1];
					  	tri->n[v].z = attrib.normals[3*idx.normal_index+2];
					  	tri->n[v] = glm::normalize(tri->n[v]);
					  	if (model_load.rotation_active)
					  	{
					  		tri->n[v] = glm::vec3(model_load.rotation * glm::vec4(tri->n[v], 1.0f));
					  	}
					}

					if (model->has_uvs)
					{
					  	tri->uv[v].x = attrib.texcoords[2*idx.texcoord_index+0];
					  	tri->uv[v].y = attrib.texcoords[2*idx.texcoord_index+1];
					}
				}
				if (!has_normals) //Generate normals (all are the same)
				{
					glm::vec3 v0v1 = tri->v[1] - tri->v[0];
					glm::vec3 v0v2 = tri->v[2] - tri->v[0];
					glm::vec3 normal = glm::normalize(glm::cross(v0v1, v0v2));
					if (model_load.rotation_active)
				  	{
				  		normal = glm::normalize(glm::vec3(model_load.rotation * glm::vec4(normal, 1.0f)));
				  	}
					tri->n[0] = normal;
					tri->n[1] = normal;
					tri->n[2] = normal;
				}
				if (!model->has_uvs)
				{
					tri->uv[0] = glm::vec2(0.0f, 0.0f);
					tri->uv[1] = glm::vec2(1.0f, 0.0f);
					tri->uv[2] = glm::vec2(1.0f, 1.0f);
				}
				model->shapes[triangle_index] = tri;
				
				//per-face material
				if (model_load.has_custom_material)
				{
					tri->material = model->materials[0];
					tri->mtl = &model->mtls[0];
				}
				else
				{
					tri->material = model->materials[shapes[s].mesh.material_ids[f]];
					tri->mtl = &model->mtls[shapes[s].mesh.material_ids[f]];
				}
				//Add to lights if emissive
				if (tri->mtl->emission != glm::vec3(0.0f))
				{
					//TODO: only diffuse area light for now
					DiffuseAreaLight dal;
					dal.shape = (Shape*)(tri);
					//NOTE: double-sided lights are not allowed??
					//dal.shape->double_sided = false;
					dal.L_emit = tri->mtl->emission;
					diffuse_area_lights.push_back(dal);
					tri->area_light_index = diffuse_area_lights.size() - 1;
				}
				//Generate bounding box
				tri->bb = BoundingBox(*tri);
			}
			triangle_index++;
			index_offset += fv;
		}
	}
	
	LOG_MESSAGE(true,
				"Successfully loaded %s:\n"
				"\t%lu spheres\n"
				"\t%lu triangles\n"
				"\t%lu shapes\n"
				"\t%lu material(s)\n"
				"\t\t%lu matte\n"
				"\t\t%lu mirror\n"
				"\t\t%lu plastic\n"
				"\t\t%lu translucent\n"
				"\t\t%lu water\n"
				"\t\t%lu glass\n",
				model_load.file.c_str(),
				model->spheres.size(),
				model->triangles.size(),
				model->shapes.size(),
				model->materials.size(),
				model->matte_materials.size(),
				model->mirror_materials.size(),
				model->plastic_materials.size(),
				model->translucent_materials.size(),
				model->water_materials.size(),
				model->glass_materials.size());
	
	return true;
}

bool Scene::LoadSphere(const SphereLoad& sphere, const unsigned int model_index)
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
		model->translucent_materials.push_back(TranslucentMaterial(sphere.transmittance));
		model->materials.push_back(&model->translucent_materials[0]);
	}
	else if (sphere.material == "water")
	{
		model->water_materials.push_back(WaterMaterial(sphere.reflectance, sphere.transmittance));
		model->materials.push_back(&model->water_materials[0]);
	}
	else if (sphere.material == "glass")
	{
		model->glass_materials.push_back(GlassMaterial(sphere.reflectance, sphere.transmittance));
		model->materials.push_back(&model->glass_materials[0]);
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
				"\t\t[%f %f %f] Ks\n"
				"\t\t[%f %f %f] Kr\n"
				"\t\t[%f %f %f] Kt\n",
				model->spheres[0].center.x, model->spheres[0].center.y, model->spheres[0].center.z,
				model->spheres[0].radius,
				sphere.material.c_str(),
				sphere.diffuse.x, sphere.diffuse.y, sphere.diffuse.z,
				sphere.specular.x, sphere.specular.y, sphere.specular.z,
				sphere.reflectance.x, sphere.reflectance.y, sphere.reflectance.z,
				sphere.transmittance.x, sphere.transmittance.y, sphere.transmittance.z);
				
	return true;
}

bool Scene::Intersect(Ray* ray, SurfaceInteraction* isect, const float t_min, const float t_max) const
{
	bool intersected = false;
	/*for (const Model& model : models)
	{
		intersected |= model.Intersect(ray, isect, t_min, t_max); //|= is logical OR
	}*/
	intersected = bvh_tree->Intersect(ray, isect);
	
	if (intersected)
	{
		isect->ray = ray;
		isect->point = ray->AtError();
		isect->normal = isect->shape->Normal(isect->Point());
		isect->wo = -ray->dir;
	}
	
	return intersected;
}
