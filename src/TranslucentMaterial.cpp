#include "BSDF.h"
#include "Logger.h"
#include "SpecularBTDF.h"
#include "TranslucentMaterial.h"

TranslucentMaterial::TranslucentMaterial(const glm::vec3& T)
{
	this->T = T;
	this->eta_outside = 1.0f;
	this->eta_inside = 1.0f;
	
	if (T == glm::vec3(0.0f))
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "T of TranslucentMaterial %p is (0,0,0)\n", this);
	}
}

void TranslucentMaterial::Info() const
{
	LOG_MESSAGE(true, "Translucent material: T=(%f %f %f), eta_ouside=%f, eta_inside=%f\n", T.x, T.y, T.z, eta_outside, eta_inside);
}

void TranslucentMaterial::ComputeScatteringFunctions(SurfaceInteraction* isect) const
{
	isect->bsdf = new BSDF();
	if (T != glm::vec3(0.0f))
	{
		isect->bsdf->Add(new SpecularBTDF(T, eta_outside, eta_inside));
	}
}
