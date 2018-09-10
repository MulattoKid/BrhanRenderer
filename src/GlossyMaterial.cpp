#include "BSDF.h"
#include "GlossyMaterial.h"
#include "LambertianBRDF.h"
#include "Logger.h"
#include "SpecularBRDF.h"

GlossyMaterial::GlossyMaterial(const glm::vec3& Kd, const glm::vec3& Ks)
{
	this->Kd = Kd;
	this->Ks = Ks;
}

void GlossyMaterial::ComputeScatteringFunctions(SurfaceInteraction* isect) const
{
	isect->bsdf = new BSDF();
	if (Kd != glm::vec3(0.0f))
	{
		isect->bsdf->Add(new LambertianBRDF(Ks));
	}
	else
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "Kd of GlossyMaterial %p is (0,0,0)\n", this);
	}
	
	if (Ks != glm::vec3(0.0f))
	{
		isect->bsdf->Add(new SpecularBRDF(Ks));
	}
	else
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "Ks of MirrorMaterial %p is (0,0,0)\n", this);
	}
}
