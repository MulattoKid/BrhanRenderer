#include "BSDF.h"
#include "Logger.h"

BSDF::BSDF()
{}

void BSDF::Add(BxDF* bxdf)
{
	if (num_bxdfs == max_bxdfs)
	{
		LOG_WARNING(false, __FILE__, __FUNCTION__, __LINE__, "The maximum number of BxDFs (%i) is already reached - skipping this new one\n", max_bxdfs);
	}
	bxdfs[num_bxdfs++] = bxdf;
}

glm::vec3 BSDF::f(const glm::vec3& wo, const glm::vec3& wi, BxDFType flags) const
{
	glm::vec3 f(0.0f);
	for (int i = 0; i < num_bxdfs; i++)
	{
		if (bxdfs[i]->MatchesFlags(flags))
		{
			f += bxdfs[i]->f(wo, wi);
		}
	}
	
	return f;
}
