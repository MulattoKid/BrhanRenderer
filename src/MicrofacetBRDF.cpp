#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "MicrofacetBRDF.h"

MicrofacetBRDF::MicrofacetBRDF(const glm::vec3& R, MicrofacetDistribution* distr, Fresnel* fresnel) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY), MICROFACET_BRDF)
{
	this->R = R;
	this->distr = distr;
	this->fresnel = fresnel;
}

MicrofacetBRDF::~MicrofacetBRDF()
{
	delete distr;
	delete fresnel;
}

float MicrofacetBRDF::Pdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal) const
{
	if (glm::dot(wo, wi) <= 0.0f) { return 0.0f; }
	glm::vec3 wh = glm::normalize(wi + wo);
	return distr->Pdf(wo, wh, normal) / (4.0f * glm::dot(wo, wh));
}

//p.547
glm::vec3 MicrofacetBRDF::f(const glm::vec3& wo, const glm::vec3& normal, const glm::vec3& wi) const
{
	float cos_theta_wo = glm::abs(glm::dot(wo, normal));
	float cos_theta_wi = glm::abs(glm::dot(wi, normal));
	glm::vec3 wh = wo + wi; //Half-way vector
	
	if (cos_theta_wo == 0.0f || cos_theta_wi == 0.0f) { return glm::vec3(0.0f); }
	if (wh == glm::vec3(0.0f)) { return glm::vec3(0.0f); }
	
	wh = glm::normalize(wh);
	glm::vec3 Fr = fresnel->Evaluate(wi, normal);
	return (R * distr->D(wh, normal) * distr->G(wo, normal, wi) * Fr) / (4.0f * cos_theta_wo * cos_theta_wi);
}

glm::vec3 MicrofacetBRDF::Samplef(const glm::vec3& wo, const float u[2], const glm::vec3& normal,
							  glm::vec3* wi, float* pdf, BxDFType* sampled_type) const
{
	glm::vec3 wh = distr->SampleWh(wo, normal, u);
	*wi = glm::reflect(-wo, wh);
	//Sampled wh (microfacet's normal) isn't in the same
	//	hemisphere as wo, and is therefore not "visible"
	if (glm::dot(wo, *wi) <= 0.0f) { return glm::vec3(0.0f); }
	*pdf = Pdf(wo, *wi, normal);
	*sampled_type = BxDFType(BSDF_REFLECTION | BSDF_GLOSSY);
	
	return f(wo, normal, *wi);
}
