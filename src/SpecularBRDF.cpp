#include "glm/geometric.hpp"
#include "SpecularBRDF.h"

SpecularBRDF::SpecularBRDF(const glm::vec3& R) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_SPECULAR))
{
	this->R = R;
}

SpecularBRDF::~SpecularBRDF()
{}

float SpecularBRDF::Pdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal) const
{
	return 0.0f;
}

glm::vec3 SpecularBRDF::f(const glm::vec3& wo, const glm::vec3& wi) const
{
	return glm::vec3(0.0f);
}

glm::vec3 SpecularBRDF::Samplef(const glm::vec3& wo, const float u[2], const glm::vec3& normal,
							    glm::vec3* wi, float* pdf, BxDFType* sampled_type) const
{
	*wi = glm::reflect(-wo, normal); //GLM expects the idnicent vector to be incoming - flip wo
	*pdf = 1.0f;
	*sampled_type = type;
	return R;
}
