#include "BSDF.h"
#include "LambertianBRDF.h"
#include "MatteMaterial.h"

MatteMaterial::MatteMaterial(const glm::vec3& Kd)
{
	this->Kd = Kd;
}

void MatteMaterial::ComputeScatteringFunctions(SurfaceInteraction* isect) const
{
	isect->bsdf = new BSDF();
	if (Kd != glm::vec3(0.0f))
	{
		isect->bsdf->Add(new LambertianBRDF(Kd));
	}
}
