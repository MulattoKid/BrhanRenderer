#include "BxDF.h"
#include "GeometricUtilities.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"

BxDF::BxDF(BxDFType type)
{
	this->type = type;
}

BxDF::~BxDF()
{}

bool BxDF::MatchesFlags(BxDFType t) const
{
	return (type & t) == type;
}

float BxDF::Pdf(const glm::vec3& wo, const glm::vec3& wi, const glm::vec3& normal) const
{
	return UniformHemispherePdf(wo, wi, normal);
}

glm::vec3 BxDF::Samplef(const glm::vec3& wo, const float u[2], const glm::vec3& normal,
						glm::vec3* wi, float* pdf, BxDFType* sampled_type) const
{
	*wi = UniformSampleHemisphere(u, normal);
	*pdf = Pdf(wo, *wi, normal);
	*sampled_type = type;
	return f(wo, normal, *wi);
}
