#include "BSDF.h"
#include "FresnelConductor.h"
#include "Logger.h"
#include "MetalData.h"
#include "MetalMaterial.h"
#include "MicrofacetBRDF.h"
#include "Spectrum.h"

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
		MicrofacetBRDF* m_ptr = (MicrofacetBRDF*)(mem_pool->Allocate(MEM_POOL_BxDF, thread_id));
		BeckmannDistribution* b_ptr = (BeckmannDistribution*)(mem_pool->Allocate(MEM_POOL_MICRO_DISTRIBUTION, thread_id));
		FresnelConductor* f_ptr = (FresnelConductor*)(mem_pool->Allocate(MEM_POOL_FRESNEL, thread_id));
		
		new(f_ptr) FresnelConductor(glm::vec3(1.0f), N, K);
		//new(b_ptr) BeckmannDistribution(MicrofacetDistribution::RoughnessToAlpha(0.01f));
		new(b_ptr) BeckmannDistribution(0.5f);
		new(m_ptr) MicrofacetBRDF(Ks, b_ptr, f_ptr);
		
		isect->bsdf->Add(m_ptr);
	}
}
