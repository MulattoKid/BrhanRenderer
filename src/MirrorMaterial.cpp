#include "BSDF.h"
#include "FresnelNoOp.h"
#include "MirrorMaterial.h"
#include "Logger.h"
#include "SpecularBRDF.h"

MirrorMaterial::MirrorMaterial(const glm::vec3& Ks)
{
	this->Ks = Ks;
	
	if (Ks == glm::vec3(0.0f))
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "Ks of MirrorMaterial %p is (0,0,0)\n", this);
	}
}

void MirrorMaterial::Info() const
{
	LOG_MESSAGE(true, "Mirror material: Ks=(%f %f %f)\n", Ks.x, Ks.y, Ks.z);
}

void MirrorMaterial::ComputeScatteringFunctions(SurfaceInteraction* isect) const
{
	isect->bsdf = new BSDF();
	if (Ks != glm::vec3(0.0f))
	{
		isect->bsdf->Add(new SpecularBRDF(Ks, new FresnelNoOp()));
	}
}
