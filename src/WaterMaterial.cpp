#include "BSDF.h"
#include "FresnelDielectric.h"
#include "Logger.h"
#include "SpecularBRDF.h"
#include "SpecularBTDF.h"
#include "WaterMaterial.h"

WaterMaterial::WaterMaterial(const glm::vec3& R, const glm::vec3& T)
{
	this->R = R;
	this->T = T;
	this->eta_outside = 1.0f;
	this->eta_inside = 1.5f;
	
	if (R == glm::vec3(0.0f))
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "R of WaterMaterial %p is (0,0,0)\n", this);
	}
	if (T == glm::vec3(0.0f))
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "T of WaterMaterial %p is (0,0,0)\n", this);
	}
}

void WaterMaterial::Info() const
{
	LOG_MESSAGE(true, "Water material: R=(%f %f %f) T=(%f %f %f), eta_ouside=%f, eta_inside=%f\n", R.x, R.y, R.z, T.x, T.y, T.z, eta_outside, eta_inside);
}

void WaterMaterial::ComputeScatteringFunctions(SurfaceInteraction* isect) const
{
	isect->bsdf = new BSDF();
	if (R != glm::vec3(0.0f))
	{
		isect->bsdf->Add(new SpecularBRDF(R, new FresnelDielectric(eta_outside, eta_inside)));
	}
	if (T != glm::vec3(0.0f))
	{
		isect->bsdf->Add(new SpecularBTDF(T, eta_outside, eta_inside));
	}
}
