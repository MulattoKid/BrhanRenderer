#include "BSDF.h"
#include "LambertianBRDF.h"
#include "Logger.h"
#include "MatteMaterial.h"
#include "OrenNayarBRDF.h"

MatteMaterial::MatteMaterial(const glm::vec3& Kd)
{
	this->Kd = Kd;
	
	if (Kd == glm::vec3(0.0f))
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "Kd of MatteMaterial %p is (0,0,0)\n", this);
	}
}

void MatteMaterial::Info() const
{
	LOG_MESSAGE(true, "Matte material: Kd=(%f %f %f)\n", Kd.x, Kd.y, Kd.z);
}

void MatteMaterial::ComputeScatteringFunctions(SurfaceInteraction* isect) const
{
	isect->bsdf = new BSDF();
	if (Kd != glm::vec3(0.0f))
	{
		isect->bsdf->Add(new LambertianBRDF(Kd));
		//isect->bsdf->Add(new OrenNayarBRDF(Kd, 20.0f));
	}
}
