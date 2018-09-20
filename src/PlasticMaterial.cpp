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

void PlasticMaterial::ComputeScatteringFunctions(SurfaceInteraction* isect) const
{
	isect->bsdf = new BSDF();
	if (Kd != glm::vec3(0.0f))
	{
		isect->bsdf->Add(new LambertianBRDF(Ks));
	}
	if (Ks != glm::vec3(0.0f))
	{
		isect->bsdf->Add(new SpecularBRDF(Ks, new FresnelNoOp()));
	}
}
