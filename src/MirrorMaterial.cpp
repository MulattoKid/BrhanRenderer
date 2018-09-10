#include "BSDF.h"
#include "MirrorMaterial.h"
#include "Logger.h"
#include "SpecularBRDF.h"

MirrorMaterial::MirrorMaterial(const glm::vec3& Ks)
{
	this->Ks = Ks;
}

void MirrorMaterial::ComputeScatteringFunctions(SurfaceInteraction* isect) const
{
	isect->bsdf = new BSDF();
	if (Ks != glm::vec3(0.0f))
	{
		isect->bsdf->Add(new SpecularBRDF(Ks));
	}
	else
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "Ks of MirrorMaterial %p is (0,0,0)\n", this);
	}
}
