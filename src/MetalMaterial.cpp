#include "BSDF.h"
#include "FresnelConductor.h"
#include "Logger.h"
#include "MetalData.h"
#include "MetalMaterial.h"
#include "Spectrum.h"
#include "SpecularBRDF.h"

MetalMaterial::MetalMaterial(const glm::vec3& Ks, const MetalType m_type)
{
	this->Ks = Ks;
	switch(m_type)
	{
		case COPPER:
			this->N = RGBFromSampled(CopperWavelengths, CopperN, CopperSamples);
			this->K = RGBFromSampled(CopperWavelengths, CopperK, CopperSamples);
			break;
		default:
			LOG_ERROR(false, __FILE__, __FUNCTION__, __LINE__, "No material type specified\n");
			break;
	}
	
	if (Ks == glm::vec3(0.0f))
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "Ks of MetalMaterial %p is (0,0,0)\n", this);
	}
}

void MetalMaterial::Info() const
{
	LOG_MESSAGE(true, "Metal material: Ks=(%f %f %f)\n", Ks.x, Ks.y, Ks.z);
}

void MetalMaterial::ComputeScatteringFunctions(SurfaceInteraction* isect, MemoryPool* mem_pool, const int thread_id) const
{
	isect->bsdf = (BSDF*)(mem_pool->Allocate(MemoryPoolObjectTypes::MEM_POOL_BSDF, thread_id));
	new(isect->bsdf) BSDF();
	
	if (Ks != glm::vec3(0.0f))
	{
		SpecularBRDF* s_ptr = (SpecularBRDF*)(mem_pool->Allocate(MEM_POOL_BxDF, thread_id));
		FresnelConductor* f_ptr = (FresnelConductor*)(mem_pool->Allocate(MEM_POOL_FRESNEL, thread_id));
		//Assuming coming from AIR -> glm::vec3(1.0f)
		new(f_ptr) FresnelConductor(glm::vec3(1.0f), N, K);
		new(s_ptr) SpecularBRDF(Ks, f_ptr, FRESNEL_CONDUCTOR);
		isect->bsdf->Add(s_ptr);
	}
}
