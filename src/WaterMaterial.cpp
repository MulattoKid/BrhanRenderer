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
		FresnelDielectric* f_ptr = new FresnelDielectric(eta_outside, eta_inside);
		SpecularBRDF* s_ptr = new SpecularBRDF(R, f_ptr, FRESNEL_DIELECTRIC);
		isect->bsdf->Add(s_ptr);
	}
	if (T != glm::vec3(0.0f))
	{
		SpecularBTDF* s_ptr = new SpecularBTDF(T, eta_outside, eta_inside);
		isect->bsdf->Add(s_ptr);
	}
}
