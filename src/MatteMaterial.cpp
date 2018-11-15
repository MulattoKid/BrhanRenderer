#include "BSDF.h"
#include "glm/vec2.hpp"
#include "LambertianBRDF.h"
#include "Logger.h"
#include "MatteMaterial.h"
#include "OrenNayarBRDF.h"
#include "Shape.h"

MatteMaterial::MatteMaterial(const glm::vec3& Kd)
{
	this->Kd = Kd;
	this->t_Kd = NULL;
	
	if (Kd == glm::vec3(0.0f))
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "Kd of MatteMaterial %p is (0,0,0)\n", this);
	}
}

MatteMaterial::MatteMaterial(const std::string& map_Kd)
{
	this->Kd = glm::vec3(0.0f);
	this->t_Kd = new Texture(map_Kd);
}

void MatteMaterial::Info() const
{
	if (t_Kd == NULL)
	{
		LOG_MESSAGE(true, "Matte material: Kd=(%f %f %f)\n", Kd.x, Kd.y, Kd.z);
	}
	else
	{
		LOG_MESSAGE(true, "Matte material: Texture=%s\n", t_Kd->file.c_str());
	}
}

void MatteMaterial::ComputeScatteringFunctions(SurfaceInteraction* isect) const
{
	isect->bsdf = new BSDF();
	
	if (t_Kd == NULL)
	{
		if (Kd != glm::vec3(0.0f))
		{
			LambertianBRDF* l_ptr = new LambertianBRDF(Kd);
			isect->bsdf->Add(l_ptr);
			
			//isect->bsdf->Add(new OrenNayarBRDF(Kd, 20.0f));
		}
	}
	else
	{	
		const glm::vec3 point(isect->point.x.f, isect->point.y.f, isect->point.z.f);
		const glm::vec2 uv = isect->shape->UV(point);

		LambertianBRDF* l_ptr = new LambertianBRDF(t_Kd->Sample(uv.x, uv.y, isect->dudx, isect->dvdx, isect->dudy, isect->dvdy));
		isect->bsdf->Add(l_ptr);
		
		//isect->bsdf->Add(new OrenNayarBRDF(t_Kd->Sample(uv.x, uv.y, 20.0f));
	}
}
