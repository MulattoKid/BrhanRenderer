#include "BSDF.h"
#include "FresnelNoOp.h"
#include "LambertianBRDF.h"
#include "Logger.h"
#include "PlasticMaterial.h"
#include "SpecularBRDF.h"

PlasticMaterial::PlasticMaterial(const glm::vec3& Kd, const glm::vec3& Ks)
{
	this->Kd = Kd;
	this->Ks = Ks;
	
	if (Kd == glm::vec3(0.0f))
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "Kd of GlossyMaterial %p is (0,0,0)\n", this);
	}
	if (Ks == glm::vec3(0.0f))
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "Ks of MirrorMaterial %p is (0,0,0)\n", this);
	}
}

void PlasticMaterial::Info() const
{
	LOG_MESSAGE(true, "Plastic material: Kd=(%f %f %f) Ks=(%f %f %f)\n", Kd.x, Kd.y, Kd.z, Ks.x, Ks.y, Ks.z);
}

void PlasticMaterial::ComputeScatteringFunctions(SurfaceInteraction* isect, MemoryPool* mem_pool, const int thread_id) const
{
	isect->bsdf = (BSDF*)(mem_pool->Allocate(sizeof(BSDF), thread_id));
	new(isect->bsdf) BSDF();
	
	if (Kd != glm::vec3(0.0f))
	{
		LambertianBRDF* l_ptr = (LambertianBRDF*)(mem_pool->Allocate(sizeof(LambertianBRDF), thread_id));
		new(l_ptr) LambertianBRDF(Kd);
		isect->bsdf->Add(l_ptr);
	}
	if (Ks != glm::vec3(0.0f))
	{
		SpecularBRDF* s_ptr = (SpecularBRDF*)(mem_pool->Allocate(sizeof(SpecularBRDF), thread_id));
		FresnelNoOp* f_ptr = (FresnelNoOp*)(mem_pool->Allocate(sizeof(FresnelNoOp), thread_id));
		new(f_ptr) FresnelNoOp();
		new(s_ptr) SpecularBRDF(Ks, f_ptr, FRESNEL_NONE);
		isect->bsdf->Add(s_ptr);
	}
}
