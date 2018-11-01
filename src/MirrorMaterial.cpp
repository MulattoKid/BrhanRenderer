#include "BSDF.h"
#include "FresnelNoOp.h"
#include "glm/vec2.hpp"
#include "MirrorMaterial.h"
#include "Logger.h"
#include "Shape.h"
#include "SpecularBRDF.h"

MirrorMaterial::MirrorMaterial(const glm::vec3& Ks)
{
	this->Ks = Ks;
	this->t_Ks = NULL;
	
	if (Ks == glm::vec3(0.0f))
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "Ks of MirrorMaterial %p is (0,0,0)\n", this);
	}
}

MirrorMaterial::MirrorMaterial(const std::string& map_Ks)
{
	this->Ks = glm::vec3(0.0f);
	this->t_Ks = new Texture(map_Ks);
}

void MirrorMaterial::Info() const
{
	if (t_Ks == NULL)
	{
		LOG_MESSAGE(true, "Mirror material: Ks=(%f %f %f)\n", Ks.x, Ks.y, Ks.z);
	}
	else
	{
		LOG_MESSAGE(true, "Mirror material: Texture=%s\n", t_Ks->file.c_str());
	}
}

void MirrorMaterial::ComputeScatteringFunctions(SurfaceInteraction* isect, MemoryPool* mem_pool, const int thread_id) const
{
	isect->bsdf = (BSDF*)(mem_pool->Allocate(sizeof(BSDF), thread_id));
	new(isect->bsdf) BSDF();
	
	if (t_Ks == NULL)
	{
		if (Ks != glm::vec3(0.0f))
		{
			SpecularBRDF* s_ptr = (SpecularBRDF*)(mem_pool->Allocate(sizeof(SpecularBRDF), thread_id));
			FresnelNoOp* f_ptr = (FresnelNoOp*)(mem_pool->Allocate(sizeof(FresnelNoOp), thread_id));
			new(f_ptr) FresnelNoOp();
			new(s_ptr) SpecularBRDF(Ks, f_ptr, FRESNEL_NONE);
			isect->bsdf->Add(s_ptr);
		}
	}
	else
	{
		glm::vec3 point(isect->point.x.f, isect->point.y.f, isect->point.z.f);
		glm::vec2 uv = isect->shape->UV(point);
		
		SpecularBRDF* s_ptr = (SpecularBRDF*)(mem_pool->Allocate(sizeof(SpecularBRDF), thread_id));
		FresnelNoOp* f_ptr = (FresnelNoOp*)(mem_pool->Allocate(sizeof(FresnelNoOp), thread_id));
		new(f_ptr) FresnelNoOp();
		new(s_ptr) SpecularBRDF(t_Ks->Sample(uv.x, uv.y, isect->dudx, isect->dvdx, isect->dudy, isect->dvdy), f_ptr, FRESNEL_NONE);
		isect->bsdf->Add(s_ptr);
	}
}
