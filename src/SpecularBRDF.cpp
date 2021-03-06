#include "glm/geometric.hpp"
#include "SpecularBRDF.h"

SpecularBRDF::SpecularBRDF(const glm::vec3& R, Fresnel* fresnel, FresnelType fresnel_type) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_SPECULAR), SPECULAR_BRDF)
{
	this->R = R;
	this->fresnel = fresnel;
	this->fresnel_type = fresnel_type;
}

SpecularBRDF::~SpecularBRDF()
{
	delete fresnel;
}

float SpecularBRDF::Pdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal) const
{
	return 0.0f;
}

glm::vec3 SpecularBRDF::f(const glm::vec3& wo, const glm::vec3& normal, const glm::vec3& wi) const
{
	return glm::vec3(0.0f);
}

glm::vec3 SpecularBRDF::Samplef(const glm::vec3& wo, const float u[2], const glm::vec3& normal,
							    glm::vec3* wi, float* pdf, BxDFType* sampled_type) const
{
	glm::vec3 normal_wo_side = normal;
	//The incident ray is coming from within the object and needs to be reflected back in
	if (glm::dot(wo, normal) < 0.0f)
	{
		normal_wo_side *= -1;
	}

	*wi = glm::reflect(-wo, normal_wo_side); //GLM expects the incident vector to be incoming -> flip wo
	*pdf = 1.0f;
	*sampled_type = type;
	return fresnel->Evaluate(*wi, normal) * R / glm::abs(glm::dot(*wi, normal_wo_side));
}
