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
		case ALUMINIUM:
			this->N = RGBFromSampled(AluminiumWavelengths, AluminiumN, AluminiumSamples);
			this->K = RGBFromSampled(AluminiumWavelengths, AluminiumK, AluminiumSamples);
			break;
		case COPPER:
			this->N = RGBFromSampled(CopperWavelengths, CopperN, CopperSamples);
			this->K = RGBFromSampled(CopperWavelengths, CopperK, CopperSamples);
			break;
		case GOLD:
			this->N = RGBFromSampled(GoldWavelengths, GoldN, GoldSamples);
			this->K = RGBFromSampled(GoldWavelengths, GoldK, GoldSamples);
			break;
		case SALT:
			this->N = RGBFromSampled(SaltWavelengths, SaltN, SaltSamples);
			this->K = RGBFromSampled(SaltWavelengths, SaltK, SaltSamples);
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

void MetalMaterial::ComputeScatteringFunctions(SurfaceInteraction* isect) const
{
	isect->bsdf = new BSDF();
	
	if (Ks != glm::vec3(0.0f))
	{
		FresnelConductor* f_ptr = new FresnelConductor(glm::vec3(1.0f), N, K);
		BeckmannDistribution* b_ptr = new BeckmannDistribution(MicrofacetDistribution::RoughnessToAlpha(0.01f));
		MicrofacetBRDF* m_ptr = new MicrofacetBRDF(Ks, b_ptr, f_ptr);
		isect->bsdf->Add(m_ptr);
	}
}
